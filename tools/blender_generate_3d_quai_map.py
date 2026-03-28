"""Blender Python: Procedural 3D marina from a circular harbor map.

Generates:
- Big water plane
- Grass on the right half (outside the circle)
- Circular basin + breakwater ring with an entrance gap
- Radial piers pointing inward

No textures are used. You can optionally load your reference image as a viewport-only
image empty (not a texture).

How to run:
- Blender -> Scripting -> Text Editor -> Open this file -> Run Script
"""

import math

import bpy
import bmesh
from mathutils import Vector


# ---------------------------
# Parameters (tweak these)
# ---------------------------

CLEAR_SCENE = True
COLLECTION_NAME = "Marina3D"

# Optional: load your map image as a reference (not used as a texture)
REFERENCE_IMAGE_PATH = None  # e.g. r"D:\\path\\to\\map.png"
REFERENCE_IMAGE_SCALE = 240.0
REFERENCE_IMAGE_Z = 0.02

# Units: treat 1.0 as ~1 meter
WATER_Z = 0.0
WATER_PLANE_SIZE = 500.0
WATER_DEPTH = 8.0

# Global depth multiplier (Z thickness/height for all generated solids)
DEPTH_MULT = 2.0

# Water mesh resolution (used so we can cut out water under the grass)
SEA_GRID_X = 220
SEA_GRID_Y = 220

# Edge smoothing quality (increases circle/grid resolution; higher = smoother but heavier)
EDGE_QUALITY = 3.0
MAX_GRID_SEGMENTS = 700
MAX_CIRCLE_SEGMENTS = 720

# Under solids: remove water and place a solid foundation
CUT_WATER_UNDER_SOLIDS = True
ADD_SOLID_FOUNDATION_UNDER_SOLIDS = True
FOUNDATION_Z = WATER_Z + 0.005
# Margin used only for cutting water under/near solids (helps avoid leftover water pixels)
WATER_CUTOUT_SOLID_MARGIN = 2.0
# Margin used for the solid foundation footprint itself (keep 0.0 so foundation matches pier length)
FOUNDATION_SOLID_MARGIN = 0.0

# Grass/land: right half of the map, outside the harbor circle
ADD_GRASS = True
GRASS_Z = WATER_Z + 0.02
GRASS_HEIGHT = 0.6
GRASS_COLOR = (0.16, 0.40, 0.18, 1.0)
GRASS_INNER_MARGIN = 0.0

# Soil under the grass (so it isn't water below the grass)
ADD_SOIL_UNDER_GRASS = True
SOIL_COLOR = (0.20, 0.12, 0.06, 1.0)
SOIL_DEPTH = 0.8
SOIL_TOP_OFFSET_FROM_GRASS = -0.002

# Grid resolution for the grass cutout (higher = smoother circle edge)
GRASS_GRID_X = 220
GRASS_GRID_Y = 220

# Main circular basin/breakwater
CENTER = (0.0, 0.0)
BASIN_RADIUS = 95.0
BREAKWATER_THICKNESS = 6.0
BREAKWATER_HEIGHT = 2.8
CIRCLE_SEGMENTS = 160

# Piers around the ring (inward pointing)
PIER_COUNT = 26
PIER_LENGTH = 16.0
PIER_WIDTH = 2.0
PIER_HEIGHT = 0.9

# Entrance gap in the ring (degrees)
# "Centered entrance": set where the opening points.
# 180 = left, 270 = bottom, 90 = top, 0 = right.
ENTRANCE_CENTER_DEG = 180.0
ENTRANCE_WIDTH_DEG = 38.0

# Piers from entrance to the sea (jetties)
ADD_ENTRANCE_PIERS = True
ENTRANCE_PIER_LENGTH = 85.0
ENTRANCE_PIER_WIDTH = 3.6
ENTRANCE_PIER_HEIGHT = BREAKWATER_HEIGHT
ENTRANCE_PIER_Z = WATER_Z + 0.01
ENTRANCE_PIER_OVERLAP = 8.0

# Central platform + bridge (optional)
ADD_CENTER_PLATFORM = True
CENTER_PLATFORM_RADIUS = 20.0
CENTER_PLATFORM_THICKNESS = 6.0
CENTER_PLATFORM_Z = WATER_Z + 0.05
BRIDGE_LENGTH = 62.0
BRIDGE_WIDTH = 4.2

# Camera
ADD_CAMERA = True
ORTHO_SCALE = 260.0
CAMERA_Z = 160.0


# ---------------------------
# Helpers
# ---------------------------


def _scaled_grid_segments(n: int) -> int:
	q = max(0.25, float(EDGE_QUALITY))
	return max(2, min(int(MAX_GRID_SEGMENTS), int(round(int(n) * q))))


def _scaled_circle_segments(n: int) -> int:
	q = max(0.25, float(EDGE_QUALITY))
	return max(24, min(int(MAX_CIRCLE_SEGMENTS), int(round(int(n) * q))))


def cleanup_scene() -> None:
	bpy.ops.object.select_all(action="SELECT")
	bpy.ops.object.delete(use_global=False)
	for _ in range(3):
		try:
			bpy.ops.outliner.orphans_purge(do_recursive=True)
		except Exception:
			break


def ensure_collection(name: str) -> bpy.types.Collection:
	col = bpy.data.collections.get(name)
	if not col:
		col = bpy.data.collections.new(name)
		bpy.context.scene.collection.children.link(col)
	return col


def link_only(obj: bpy.types.Object, col: bpy.types.Collection) -> None:
	for c in list(obj.users_collection):
		c.objects.unlink(obj)
	col.objects.link(obj)


def make_mat(name: str, rgba, rough=0.6, metallic=0.0) -> bpy.types.Material:
	mat = bpy.data.materials.get(name) or bpy.data.materials.new(name)
	mat.use_nodes = True
	nodes = mat.node_tree.nodes
	links = mat.node_tree.links
	nodes.clear()
	out = nodes.new("ShaderNodeOutputMaterial")
	bsdf = nodes.new("ShaderNodeBsdfPrincipled")
	bsdf.inputs["Base Color"].default_value = rgba
	bsdf.inputs["Roughness"].default_value = rough
	bsdf.inputs["Metallic"].default_value = metallic
	links.new(bsdf.outputs["BSDF"], out.inputs["Surface"])
	mat.diffuse_color = rgba
	return mat


def make_grass_mat(name: str, base_rgba) -> bpy.types.Material:
	"""Procedural grass material (no external textures).

	Creates small-scale color variation + subtle bump so the grass reads as organic.
	"""
	mat = bpy.data.materials.get(name) or bpy.data.materials.new(name)
	mat.use_nodes = True
	nodes = mat.node_tree.nodes
	links = mat.node_tree.links
	nodes.clear()

	out = nodes.new("ShaderNodeOutputMaterial")
	out.location = (520, 0)

	bsdf = nodes.new("ShaderNodeBsdfPrincipled")
	bsdf.location = (280, 0)
	bsdf.inputs["Roughness"].default_value = 0.85
	# Blender version compatibility: specular socket name changed across versions
	if "Specular" in bsdf.inputs:
		bsdf.inputs["Specular"].default_value = 0.25
	elif "Specular IOR Level" in bsdf.inputs:
		bsdf.inputs["Specular IOR Level"].default_value = 0.25

	texcoord = nodes.new("ShaderNodeTexCoord")
	texcoord.location = (-760, 0)

	mapping = nodes.new("ShaderNodeMapping")
	mapping.location = (-560, 0)
	# Tune overall grass "scale"
	mapping.inputs["Scale"].default_value = (6.0, 6.0, 6.0)

	noise_fine = nodes.new("ShaderNodeTexNoise")
	noise_fine.location = (-320, 120)
	noise_fine.inputs["Scale"].default_value = 55.0
	noise_fine.inputs["Detail"].default_value = 8.0
	noise_fine.inputs["Roughness"].default_value = 0.55

	noise_patch = nodes.new("ShaderNodeTexNoise")
	noise_patch.location = (-320, -120)
	noise_patch.inputs["Scale"].default_value = 8.0
	noise_patch.inputs["Detail"].default_value = 6.0
	noise_patch.inputs["Roughness"].default_value = 0.45

	cr_patch = nodes.new("ShaderNodeValToRGB")
	cr_patch.location = (-80, -120)
	# Darker/lighter patches
	cr_patch.color_ramp.elements[0].position = 0.35
	cr_patch.color_ramp.elements[1].position = 0.75
	cr_patch.color_ramp.elements[0].color = (0.10, 0.22, 0.10, 1.0)
	cr_patch.color_ramp.elements[1].color = (0.20, 0.44, 0.18, 1.0)

	cr_fine = nodes.new("ShaderNodeValToRGB")
	cr_fine.location = (-80, 120)
	# Thin blade-like variation
	cr_fine.color_ramp.elements[0].position = 0.40
	cr_fine.color_ramp.elements[1].position = 0.68
	cr_fine.color_ramp.elements[0].color = (0.12, 0.28, 0.12, 1.0)
	cr_fine.color_ramp.elements[1].color = (0.28, 0.56, 0.22, 1.0)

	mix = nodes.new("ShaderNodeMixRGB")
	mix.location = (120, 0)
	mix.blend_type = "MULTIPLY"
	mix.inputs["Fac"].default_value = 0.6

	bump = nodes.new("ShaderNodeBump")
	bump.location = (120, -220)
	bump.inputs["Strength"].default_value = 0.25
	bump.inputs["Distance"].default_value = 0.15

	# Base color bias from user parameter
	base_rgb = (float(base_rgba[0]), float(base_rgba[1]), float(base_rgba[2]), 1.0)
	bsdf.inputs["Base Color"].default_value = base_rgb

	links.new(texcoord.outputs["Object"], mapping.inputs["Vector"])
	links.new(mapping.outputs["Vector"], noise_fine.inputs["Vector"])
	links.new(mapping.outputs["Vector"], noise_patch.inputs["Vector"])

	links.new(noise_fine.outputs["Fac"], cr_fine.inputs["Fac"])
	links.new(noise_patch.outputs["Fac"], cr_patch.inputs["Fac"])

	links.new(cr_fine.outputs["Color"], mix.inputs["Color1"])
	links.new(cr_patch.outputs["Color"], mix.inputs["Color2"])
	links.new(mix.outputs["Color"], bsdf.inputs["Base Color"])

	links.new(noise_fine.outputs["Fac"], bump.inputs["Height"])
	links.new(bump.outputs["Normal"], bsdf.inputs["Normal"])

	links.new(bsdf.outputs["BSDF"], out.inputs["Surface"])
	mat.diffuse_color = base_rgb
	return mat


def make_water_mat(name: str, base_rgba) -> bpy.types.Material:
	"""Procedural water material (no external textures).

	Adds subtle wave-like normal variation + depthy tint.
	"""
	mat = bpy.data.materials.get(name) or bpy.data.materials.new(name)
	mat.use_nodes = True
	nodes = mat.node_tree.nodes
	links = mat.node_tree.links
	nodes.clear()

	out = nodes.new("ShaderNodeOutputMaterial")
	out.location = (520, 0)

	bsdf = nodes.new("ShaderNodeBsdfPrincipled")
	bsdf.location = (260, 0)
	bsdf.inputs["Base Color"].default_value = (
		float(base_rgba[0]),
		float(base_rgba[1]),
		float(base_rgba[2]),
		1.0,
	)
	bsdf.inputs["Roughness"].default_value = 0.06
	# Specular socket name varies by Blender version
	if "Specular" in bsdf.inputs:
		bsdf.inputs["Specular"].default_value = 0.55
	elif "Specular IOR Level" in bsdf.inputs:
		bsdf.inputs["Specular IOR Level"].default_value = 0.55

	texcoord = nodes.new("ShaderNodeTexCoord")
	texcoord.location = (-820, 0)

	mapping = nodes.new("ShaderNodeMapping")
	mapping.location = (-620, 0)
	mapping.inputs["Scale"].default_value = (0.22, 0.22, 0.22)

	noise = nodes.new("ShaderNodeTexNoise")
	noise.location = (-360, 80)
	noise.inputs["Scale"].default_value = 6.0
	noise.inputs["Detail"].default_value = 10.0
	noise.inputs["Roughness"].default_value = 0.35

	wave = nodes.new("ShaderNodeTexWave")
	wave.location = (-360, -120)
	wave.wave_type = "RINGS"
	wave.inputs["Scale"].default_value = 1.8
	wave.inputs["Distortion"].default_value = 6.0
	wave.inputs["Detail"].default_value = 2.0
	wave.inputs["Detail Scale"].default_value = 1.2

	mix = nodes.new("ShaderNodeMixRGB")
	mix.location = (-120, -10)
	mix.blend_type = "ADD"
	mix.inputs["Fac"].default_value = 0.65

	bump = nodes.new("ShaderNodeBump")
	bump.location = (60, -200)
	bump.inputs["Strength"].default_value = 0.35
	bump.inputs["Distance"].default_value = 0.6

	links.new(texcoord.outputs["Object"], mapping.inputs["Vector"])
	links.new(mapping.outputs["Vector"], noise.inputs["Vector"])
	links.new(mapping.outputs["Vector"], wave.inputs["Vector"])
	links.new(noise.outputs["Fac"], mix.inputs["Color1"])
	links.new(wave.outputs["Color"], mix.inputs["Color2"])
	links.new(mix.outputs["Color"], bump.inputs["Height"])
	links.new(bump.outputs["Normal"], bsdf.inputs["Normal"])
	links.new(bsdf.outputs["BSDF"], out.inputs["Surface"])

	mat.diffuse_color = (
		float(base_rgba[0]),
		float(base_rgba[1]),
		float(base_rgba[2]),
		1.0,
	)
	return mat


def make_soil_mat(name: str, base_rgba) -> bpy.types.Material:
	"""Procedural soil material (no external textures)."""
	mat = bpy.data.materials.get(name) or bpy.data.materials.new(name)
	mat.use_nodes = True
	nodes = mat.node_tree.nodes
	links = mat.node_tree.links
	nodes.clear()

	out = nodes.new("ShaderNodeOutputMaterial")
	out.location = (520, 0)

	bsdf = nodes.new("ShaderNodeBsdfPrincipled")
	bsdf.location = (260, 0)
	bsdf.inputs["Roughness"].default_value = 0.95
	if "Specular" in bsdf.inputs:
		bsdf.inputs["Specular"].default_value = 0.15
	elif "Specular IOR Level" in bsdf.inputs:
		bsdf.inputs["Specular IOR Level"].default_value = 0.15

	texcoord = nodes.new("ShaderNodeTexCoord")
	texcoord.location = (-820, 0)

	mapping = nodes.new("ShaderNodeMapping")
	mapping.location = (-620, 0)
	mapping.inputs["Scale"].default_value = (3.5, 3.5, 3.5)

	noise = nodes.new("ShaderNodeTexNoise")
	noise.location = (-360, 60)
	noise.inputs["Scale"].default_value = 10.0
	noise.inputs["Detail"].default_value = 8.0
	noise.inputs["Roughness"].default_value = 0.65

	cr = nodes.new("ShaderNodeValToRGB")
	cr.location = (-120, 60)
	cr.color_ramp.elements[0].position = 0.35
	cr.color_ramp.elements[1].position = 0.80
	# Dark to light soil tones
	cr.color_ramp.elements[0].color = (0.09, 0.05, 0.02, 1.0)
	cr.color_ramp.elements[1].color = (0.28, 0.18, 0.10, 1.0)

	mix = nodes.new("ShaderNodeMixRGB")
	mix.location = (40, 30)
	mix.blend_type = "MIX"
	mix.inputs["Fac"].default_value = 0.55

	bump = nodes.new("ShaderNodeBump")
	bump.location = (60, -200)
	bump.inputs["Strength"].default_value = 0.55
	bump.inputs["Distance"].default_value = 0.35

	base_rgb = (float(base_rgba[0]), float(base_rgba[1]), float(base_rgba[2]), 1.0)

	links.new(texcoord.outputs["Object"], mapping.inputs["Vector"])
	links.new(mapping.outputs["Vector"], noise.inputs["Vector"])
	links.new(noise.outputs["Fac"], cr.inputs["Fac"])
	# Mix ramped variation with the user base color
	links.new(cr.outputs["Color"], mix.inputs["Color1"])
	mix.inputs["Color2"].default_value = base_rgb
	links.new(mix.outputs["Color"], bsdf.inputs["Base Color"])
	links.new(noise.outputs["Fac"], bump.inputs["Height"])
	links.new(bump.outputs["Normal"], bsdf.inputs["Normal"])
	links.new(bsdf.outputs["BSDF"], out.inputs["Surface"])

	mat.diffuse_color = base_rgb
	return mat


def add_box(name: str, location, size_xyz) -> bpy.types.Object:
	# size_xyz is full size (sx, sy, sz)
	bpy.ops.mesh.primitive_cube_add(size=1.0, location=location)
	obj = bpy.context.active_object
	obj.name = name
	obj.scale = (size_xyz[0] / 2.0, size_xyz[1] / 2.0, size_xyz[2] / 2.0)
	return obj


def annulus_mesh(name: str, center_xy, r_outer: float, r_inner: float, z: float, segments: int) -> bpy.types.Object:
	cx, cy = center_xy
	verts = []
	for i in range(segments):
		a = 2.0 * math.pi * i / segments
		verts.append((cx + r_outer * math.cos(a), cy + r_outer * math.sin(a), z))
	for i in range(segments):
		a = 2.0 * math.pi * i / segments
		verts.append((cx + r_inner * math.cos(a), cy + r_inner * math.sin(a), z))

	faces = []
	for i in range(segments):
		o0 = i
		o1 = (i + 1) % segments
		i0 = segments + i
		i1 = segments + ((i + 1) % segments)
		faces.append([o0, o1, i1, i0])

	mesh = bpy.data.meshes.new(name)
	obj = bpy.data.objects.new(name, mesh)
	bpy.context.collection.objects.link(obj)
	mesh.from_pydata(verts, [], faces)
	mesh.update()
	return obj


def extrude_mesh_z(obj: bpy.types.Object, height: float) -> None:
	me = obj.data
	bm = bmesh.new()
	bm.from_mesh(me)

	if not bm.faces:
		bm.free()
		return

	ret = bmesh.ops.extrude_face_region(bm, geom=bm.faces[:])
	verts = [e for e in ret["geom"] if isinstance(e, bmesh.types.BMVert)]
	bmesh.ops.translate(bm, verts=verts, vec=Vector((0, 0, height)))

	bm.to_mesh(me)
	bm.free()
	me.update()


def _point_in_oriented_rect(px: float, py: float, cx: float, cy: float, length: float, width: float, angle_rad: float) -> bool:
	# Rectangle local +X is along angle_rad
	hl = length * 0.5
	hw = width * 0.5
	if hl <= 0.0 or hw <= 0.0:
		return False
	# Translate to center
	dx = px - cx
	dy = py - cy
	ca = math.cos(-angle_rad)
	sa = math.sin(-angle_rad)
	lx = dx * ca - dy * sa
	ly = dx * sa + dy * ca
	return (abs(lx) <= hl) and (abs(ly) <= hw)


def _is_under_solid_xy(px: float, py: float, margin: float = 0.0) -> bool:
	"""Return True if (px,py) lies under any solid structure footprint.

	Margin expands footprints in XY (used for water-cutout robustness).
	"""
	cx, cy = CENTER
	margin = max(0.0, float(margin))

	# Breakwater ring footprint (respect entrance gap)
	dx = px - cx
	dy = py - cy
	r2 = dx * dx + dy * dy
	r_in = max(0.0, BASIN_RADIUS - margin)
	r_out = BASIN_RADIUS + BREAKWATER_THICKNESS + margin
	if (r_in * r_in) <= r2 <= (r_out * r_out):
		ang = math.degrees(math.atan2(py - cy, px - cx)) % 360.0
		if not in_entrance_gap(ang):
			return True

	# Central platform annulus footprint
	if ADD_CENTER_PLATFORM:
		pr_in = max(0.0, (CENTER_PLATFORM_RADIUS - CENTER_PLATFORM_THICKNESS) - margin)
		pr_out = max(0.0, CENTER_PLATFORM_RADIUS + margin)
		if (pr_in * pr_in) <= r2 <= (pr_out * pr_out):
			return True

	# Center bridges/piers (two)
	if ADD_CENTER_PLATFORM:
		offset = BASIN_RADIUS * 0.25
		bridge_z = WATER_Z + 0.05  # only used for Z placement elsewhere
		_ = bridge_z
		# Right
		rx = cx + offset + BRIDGE_LENGTH * 0.5
		ry_ = cy
		if _point_in_oriented_rect(px, py, rx, ry_, BRIDGE_LENGTH + 2.0 * margin, BRIDGE_WIDTH + 2.0 * margin, 0.0):
			return True
		# Left
		lx = cx - offset - BRIDGE_LENGTH * 0.5
		ly_ = cy
		if _point_in_oriented_rect(px, py, lx, ly_, BRIDGE_LENGTH + 2.0 * margin, BRIDGE_WIDTH + 2.0 * margin, 0.0):
			return True

	# Entrance jetties (two), aligned with entrance center direction
	if ADD_ENTRANCE_PIERS and ENTRANCE_PIER_LENGTH > 0.0 and ENTRANCE_PIER_WIDTH > 0.0:
		center_rad = math.radians(ENTRANCE_CENTER_DEG)
		half_gap = ENTRANCE_WIDTH_DEG * 0.5
		edge_angles = [ENTRANCE_CENTER_DEG - half_gap, ENTRANCE_CENTER_DEG + half_gap]
		r_start = BASIN_RADIUS + (BREAKWATER_THICKNESS * 0.5)
		x_at_entrance = cx + r_start * math.cos(center_rad)
		for ang_deg in edge_angles:
			a = math.radians(ang_deg)
			y_at_edge = cy + r_start * math.sin(a)
			start_x = x_at_entrance - math.cos(center_rad) * float(ENTRANCE_PIER_OVERLAP)
			start_y = y_at_edge - math.sin(center_rad) * float(ENTRANCE_PIER_OVERLAP)
			center_x = start_x + math.cos(center_rad) * (ENTRANCE_PIER_LENGTH * 0.5)
			center_y = start_y + math.sin(center_rad) * (ENTRANCE_PIER_LENGTH * 0.5)
			if _point_in_oriented_rect(
				px,
				py,
				center_x,
				center_y,
				ENTRANCE_PIER_LENGTH + 2.0 * margin,
				ENTRANCE_PIER_WIDTH + 2.0 * margin,
				center_rad,
			):
				return True

	# Radial piers footprint
	if PIER_COUNT > 0 and PIER_LENGTH > 0.0 and PIER_WIDTH > 0.0:
		for i in range(PIER_COUNT):
			ang = (360.0 * i / PIER_COUNT) % 360.0
			if in_entrance_gap(ang):
				continue
			a = math.radians(ang)
			r0 = BASIN_RADIUS - 2.0
			px0 = cx + r0 * math.cos(a)
			py0 = cy + r0 * math.sin(a)
			inward_x = -math.cos(a)
			inward_y = -math.sin(a)
			ccx = px0 + inward_x * (PIER_LENGTH * 0.5)
			ccy = py0 + inward_y * (PIER_LENGTH * 0.5)
			if _point_in_oriented_rect(px, py, ccx, ccy, PIER_LENGTH + 2.0 * margin, PIER_WIDTH + 2.0 * margin, a):
				return True

	return False


def add_solid_foundation_under_solids(name: str, col: bpy.types.Collection, z: float, depth: float) -> bpy.types.Object | None:
	"""Create a solid volume under all solid structures, so there isn't water below them."""
	width = WATER_PLANE_SIZE
	height = WATER_PLANE_SIZE
	if width <= 1e-6 or height <= 1e-6 or depth <= 0.0:
		return None

	x_segments = _scaled_grid_segments(int(SEA_GRID_X))
	y_segments = _scaled_grid_segments(int(SEA_GRID_Y))

	bm = bmesh.new()
	bmesh.ops.create_grid(bm, x_segments=x_segments, y_segments=y_segments, size=1.0)

	half_w = width * 0.5
	half_h = height * 0.5
	bmesh.ops.scale(bm, verts=bm.verts[:], vec=Vector((half_w, half_h, 1.0)))
	bmesh.ops.translate(bm, verts=bm.verts[:], vec=Vector((CENTER[0], CENTER[1], z)))

	faces_to_delete = []
	for f in bm.faces:
		p = sum((v.co for v in f.verts), Vector()) / len(f.verts)
		if not _is_under_solid_xy(float(p.x), float(p.y), margin=FOUNDATION_SOLID_MARGIN):
			faces_to_delete.append(f)

	if faces_to_delete:
		bmesh.ops.delete(bm, geom=faces_to_delete, context="FACES")

	if not bm.faces:
		bm.free()
		return None

	mesh = bpy.data.meshes.new(name)
	obj = bpy.data.objects.new(name, mesh)
	bpy.context.collection.objects.link(obj)
	bm.to_mesh(mesh)
	bm.free()
	mesh.update()
	link_only(obj, col)
	# Extrude downward to create volume
	extrude_mesh_z(obj, -depth)
	return obj


def add_water_plane_with_grass_cutout(name: str, col: bpy.types.Collection) -> bpy.types.Object | None:
	"""Create the water plane as a grid and remove faces where grass exists.

	This ensures there is no water under the grass area.
	"""
	half = WATER_PLANE_SIZE * 0.5
	width = WATER_PLANE_SIZE
	height = WATER_PLANE_SIZE
	if width <= 1e-6 or height <= 1e-6:
		return None

	x_segments = _scaled_grid_segments(int(SEA_GRID_X))
	y_segments = _scaled_grid_segments(int(SEA_GRID_Y))

	inner_r = BASIN_RADIUS + BREAKWATER_THICKNESS + GRASS_INNER_MARGIN
	inner_r2 = inner_r * inner_r
	cx, cy = CENTER

	bm = bmesh.new()
	bmesh.ops.create_grid(bm, x_segments=x_segments, y_segments=y_segments, size=1.0)

	half_w = width * 0.5
	half_h = height * 0.5
	bmesh.ops.scale(bm, verts=bm.verts[:], vec=Vector((half_w, half_h, 1.0)))
	bmesh.ops.translate(bm, verts=bm.verts[:], vec=Vector((CENTER[0], CENTER[1], WATER_Z)))

	faces_to_delete = []
	for f in bm.faces:
		p = sum((v.co for v in f.verts), Vector()) / len(f.verts)
		# Grass exists on the right half-plane and outside the harbor circle
		if p.x < cx:
			# Keep water on the left half, unless we cut under solids
			if CUT_WATER_UNDER_SOLIDS and _is_under_solid_xy(float(p.x), float(p.y), margin=WATER_CUTOUT_SOLID_MARGIN):
				faces_to_delete.append(f)
			continue
		dx = p.x - cx
		dy = p.y - cy
		if (dx * dx + dy * dy) <= inner_r2:
			# Keep water inside the harbor circle, unless we cut under solids
			if CUT_WATER_UNDER_SOLIDS and _is_under_solid_xy(float(p.x), float(p.y), margin=WATER_CUTOUT_SOLID_MARGIN):
				faces_to_delete.append(f)
			continue
		# Right half outside circle: remove water (grass region)
		faces_to_delete.append(f)

	if faces_to_delete:
		bmesh.ops.delete(bm, geom=faces_to_delete, context="FACES")

	if not bm.faces:
		bm.free()
		return None

	mesh = bpy.data.meshes.new(name)
	obj = bpy.data.objects.new(name, mesh)
	bpy.context.collection.objects.link(obj)
	bm.to_mesh(mesh)
	bm.free()
	mesh.update()
	link_only(obj, col)
	return obj


def add_grass_right_half_outside_circle(name: str, col: bpy.types.Collection) -> bpy.types.Object | None:
	"""Create a grass slab on the right half-plane, with a circular hole cut out.

	Implementation avoids booleans: we create a grid and delete faces inside the circle.
	"""
	# Right half of the water plane: x in [CENTER.x, CENTER.x + half]
	half = WATER_PLANE_SIZE * 0.5
	width = half
	height = WATER_PLANE_SIZE
	if width <= 1e-6 or height <= 1e-6:
		return None

	x_segments = _scaled_grid_segments(int(GRASS_GRID_X))
	y_segments = _scaled_grid_segments(int(GRASS_GRID_Y))

	inner_r = BASIN_RADIUS + BREAKWATER_THICKNESS + GRASS_INNER_MARGIN
	inner_r2 = inner_r * inner_r

	bm = bmesh.new()
	bmesh.ops.create_grid(bm, x_segments=x_segments, y_segments=y_segments, size=1.0)

	half_w = width * 0.5
	half_h = height * 0.5
	bmesh.ops.scale(bm, verts=bm.verts[:], vec=Vector((half_w, half_h, 1.0)))
	bmesh.ops.translate(
		bm,
		verts=bm.verts[:],
		vec=Vector((CENTER[0] + half_w, CENTER[1], GRASS_Z)),
	)

	faces_to_delete = []
	cx, cy = CENTER
	for f in bm.faces:
		p = sum((v.co for v in f.verts), Vector()) / len(f.verts)
		# Safety: ensure only the right half remains
		if p.x < cx:
			faces_to_delete.append(f)
			continue
		dx = p.x - cx
		dy = p.y - cy
		if (dx * dx + dy * dy) <= inner_r2:
			faces_to_delete.append(f)

	if faces_to_delete:
		bmesh.ops.delete(bm, geom=faces_to_delete, context="FACES")

	if not bm.faces:
		bm.free()
		return None

	mesh = bpy.data.meshes.new(name)
	obj = bpy.data.objects.new(name, mesh)
	bpy.context.collection.objects.link(obj)
	bm.to_mesh(mesh)
	bm.free()
	mesh.update()
	link_only(obj, col)
	return obj


def add_reference_image(path: str | None, col: bpy.types.Collection) -> bpy.types.Object | None:
	if not path:
		return None
	try:
		img = bpy.data.images.load(path, check_existing=True)
	except Exception as e:
		print("Failed to load reference image:", e)
		return None

	bpy.ops.object.empty_add(type="IMAGE", location=(CENTER[0], CENTER[1], REFERENCE_IMAGE_Z))
	obj = bpy.context.active_object
	obj.name = "ReferenceMap"
	obj.empty_display_size = 1.0
	obj.data = img
	obj.scale = (REFERENCE_IMAGE_SCALE, REFERENCE_IMAGE_SCALE, REFERENCE_IMAGE_SCALE)
	link_only(obj, col)
	return obj


def _angle_delta_deg(a: float, b: float) -> float:
	# shortest signed delta a-b in degrees
	return (a - b + 180.0) % 360.0 - 180.0


def in_entrance_gap(angle_deg: float) -> bool:
	d = _angle_delta_deg(angle_deg, ENTRANCE_CENTER_DEG)
	return abs(d) <= (ENTRANCE_WIDTH_DEG * 0.5)


def clamp_angle_out_of_gap(angle_deg: float, margin_deg: float = 2.0) -> float:
	"""If an angle is inside the entrance gap, clamp it to the nearest gap edge."""

	d = _angle_delta_deg(angle_deg, ENTRANCE_CENTER_DEG)
	half = ENTRANCE_WIDTH_DEG * 0.5
	if abs(d) <= half:
		edge = half + margin_deg
		return (ENTRANCE_CENTER_DEG + (edge if d >= 0 else -edge)) % 360.0
	return angle_deg % 360.0


# ---------------------------
# Build
# ---------------------------


def build() -> None:
	if bpy.context.mode != "OBJECT":
		try:
			bpy.ops.object.mode_set(mode="OBJECT")
		except Exception:
			pass

	if CLEAR_SCENE:
		cleanup_scene()

	col = ensure_collection(COLLECTION_NAME)

	# Materials
	mat_water = make_water_mat("MAT_Water", (0.03, 0.22, 0.28, 1.0))
	mat_grass = make_grass_mat("MAT_Grass", GRASS_COLOR)
	mat_soil = make_soil_mat("MAT_Soil", SOIL_COLOR)
	mat_basin = make_mat("MAT_Basin", (0.05, 0.25, 0.30, 1.0), rough=0.12)
	mat_break = make_mat("MAT_Breakwater", (0.55, 0.54, 0.52, 1.0), rough=0.9)
	mat_pier = make_mat("MAT_Pier", (0.80, 0.80, 0.82, 1.0), rough=0.5)

	created: list[bpy.types.Object] = []

	depth_mult = max(0.01, float(DEPTH_MULT))
	grass_height = GRASS_HEIGHT * depth_mult
	breakwater_height = BREAKWATER_HEIGHT * depth_mult
	pier_height = PIER_HEIGHT * depth_mult
	entrance_pier_height = ENTRANCE_PIER_HEIGHT * depth_mult
	center_platform_height = 0.8 * depth_mult
	soil_top_z = max(WATER_Z + 0.001, GRASS_Z + float(SOIL_TOP_OFFSET_FROM_GRASS))
	water_depth = max(0.0, float(WATER_DEPTH)) * depth_mult
	soil_depth = water_depth
	foundation_z = float(FOUNDATION_Z)
	circle_segments = _scaled_circle_segments(int(CIRCLE_SEGMENTS))
	center_platform_segments = _scaled_circle_segments(96)

	# Big water plane (cut out under grass so it's not water below grass)
	sea = add_water_plane_with_grass_cutout("Sea", col)
	if sea:
		sea.data.materials.append(mat_water)
		if water_depth > 0.0:
			extrude_mesh_z(sea, -water_depth)
		created.append(sea)

	# Solid foundation under all solid structures (so no water under them)
	if ADD_SOLID_FOUNDATION_UNDER_SOLIDS and water_depth > 0.0:
		foundation = add_solid_foundation_under_solids("StructureFoundation", col, z=foundation_z, depth=water_depth)
		if foundation:
			foundation.data.materials.append(mat_break)
			created.append(foundation)

	# Grass on the right half, outside the circle
	if ADD_GRASS:
		grass = add_grass_right_half_outside_circle("GrassLand", col)
		if grass:
			# Soil below the grass footprint
			if ADD_SOIL_UNDER_GRASS:
				soil_mesh = grass.data.copy()
				soil = bpy.data.objects.new("SoilLand", soil_mesh)
				bpy.context.collection.objects.link(soil)
				soil.location.z = soil_top_z - GRASS_Z
				link_only(soil, col)
				extrude_mesh_z(soil, -soil_depth)
				soil.data.materials.append(mat_soil)
				created.append(soil)

			extrude_mesh_z(grass, grass_height)
			grass.data.materials.append(mat_grass)
			created.append(grass)

	# Reference
	ref = add_reference_image(REFERENCE_IMAGE_PATH, col)
	if ref:
		created.append(ref)

	# Basin surface (filled circle)
	basin_mesh = bpy.data.meshes.new("BasinSurface")
	basin = bpy.data.objects.new("BasinSurface", basin_mesh)
	bpy.context.collection.objects.link(basin)
	bm = bmesh.new()
	bmesh.ops.create_circle(bm, segments=circle_segments, radius=BASIN_RADIUS - (BREAKWATER_THICKNESS * 0.4))
	bmesh.ops.contextual_create(bm, geom=bm.edges[:])
	bmesh.ops.translate(bm, verts=bm.verts[:], vec=Vector((CENTER[0], CENTER[1], WATER_Z + 0.01)))
	bm.to_mesh(basin_mesh)
	bm.free()
	basin.data.materials.append(mat_basin)
	link_only(basin, col)
	created.append(basin)

	# Breakwater ring wall with entrance gap
	ring = annulus_mesh(
		"BreakwaterRing",
		CENTER,
		r_outer=BASIN_RADIUS + BREAKWATER_THICKNESS,
		r_inner=BASIN_RADIUS,
		z=WATER_Z + 0.01,
		segments=circle_segments,
	)

	bm = bmesh.new()
	bm.from_mesh(ring.data)
	faces_to_delete = []
	for f in bm.faces:
		p = sum((v.co for v in f.verts), Vector()) / len(f.verts)
		ang = math.degrees(math.atan2(p.y - CENTER[1], p.x - CENTER[0])) % 360.0
		if in_entrance_gap(ang):
			faces_to_delete.append(f)
	if faces_to_delete:
		bmesh.ops.delete(bm, geom=faces_to_delete, context="FACES")
	bm.to_mesh(ring.data)
	bm.free()

	extrude_mesh_z(ring, breakwater_height)
	ring.data.materials.append(mat_break)
	link_only(ring, col)
	created.append(ring)

	# Entrance piers/jetties extending out to the sea
	if ADD_ENTRANCE_PIERS and ENTRANCE_PIER_LENGTH > 0.0 and ENTRANCE_PIER_WIDTH > 0.0:
		base_z = float(ENTRANCE_PIER_Z)
		center_rad = math.radians(ENTRANCE_CENTER_DEG)
		dir_center = Vector((math.cos(center_rad), math.sin(center_rad), 0.0))

		half_gap = ENTRANCE_WIDTH_DEG * 0.5
		edge_angles = [ENTRANCE_CENTER_DEG - half_gap, ENTRANCE_CENTER_DEG + half_gap]

		# Place the jetties near the entrance opening, but aligned horizontally
		r_start = BASIN_RADIUS + (BREAKWATER_THICKNESS * 0.5)
		x_at_entrance = CENTER[0] + r_start * math.cos(center_rad)
		for idx, ang_deg in enumerate(edge_angles, start=1):
			a = math.radians(ang_deg)
			# Use the gap edge to position the jetty in Y, but keep it horizontal in X
			y_at_edge = CENTER[1] + r_start * math.sin(a)
			start_xy = Vector((x_at_entrance, y_at_edge, 0.0))
			# Push inward so the jetty overlaps the breakwater (reads as connected)
			overlap = max(0.0, float(ENTRANCE_PIER_OVERLAP))
			start_xy = start_xy - dir_center * overlap
			center_xy = start_xy + dir_center * (ENTRANCE_PIER_LENGTH * 0.5)
			jetty = add_box(
				f"EntranceJetty_{idx}",
				location=(center_xy.x, center_xy.y, base_z + entrance_pier_height / 2.0),
				size_xyz=(ENTRANCE_PIER_LENGTH, ENTRANCE_PIER_WIDTH, entrance_pier_height),
			)
			jetty.rotation_euler = (0.0, 0.0, center_rad)
			jetty.data.materials.append(mat_break)
			link_only(jetty, col)
			created.append(jetty)

	# Central platform + bridge
	if ADD_CENTER_PLATFORM:
		center_ring = annulus_mesh(
			"CenterPlatform",
			CENTER,
			r_outer=CENTER_PLATFORM_RADIUS,
			r_inner=CENTER_PLATFORM_RADIUS - CENTER_PLATFORM_THICKNESS,
			z=CENTER_PLATFORM_Z,
			segments=center_platform_segments,
		)
		extrude_mesh_z(center_ring, center_platform_height)
		center_ring.data.materials.append(mat_pier)
		link_only(center_ring, col)
		created.append(center_ring)

		# Two bridges/piers from the center (right + left)
		bridge_z = WATER_Z + 0.05
		offset = BASIN_RADIUS * 0.25

		r_start = Vector((CENTER[0] + offset, CENTER[1], bridge_z))
		bridge_r = add_box(
			"Bridge_Right",
			location=(r_start.x + BRIDGE_LENGTH / 2.0, r_start.y, r_start.z + pier_height / 2.0),
			size_xyz=(BRIDGE_LENGTH, BRIDGE_WIDTH, pier_height),
		)
		bridge_r.data.materials.append(mat_pier)
		link_only(bridge_r, col)
		created.append(bridge_r)

		l_start = Vector((CENTER[0] - offset, CENTER[1], bridge_z))
		bridge_l = add_box(
			"Bridge_Left",
			location=(l_start.x - BRIDGE_LENGTH / 2.0, l_start.y, l_start.z + pier_height / 2.0),
			size_xyz=(BRIDGE_LENGTH, BRIDGE_WIDTH, pier_height),
		)
		bridge_l.data.materials.append(mat_pier)
		link_only(bridge_l, col)
		created.append(bridge_l)

	# Radial piers (skip the entrance sector)
	for i in range(PIER_COUNT):
		ang = (360.0 * i / PIER_COUNT) % 360.0
		if in_entrance_gap(ang):
			continue

		a = math.radians(ang)
		r0 = BASIN_RADIUS - 2.0
		px = CENTER[0] + r0 * math.cos(a)
		py = CENTER[1] + r0 * math.sin(a)

		inward = Vector((-math.cos(a), -math.sin(a), 0.0))
		center_pt = Vector((px, py, WATER_Z + 0.05)) + inward * (PIER_LENGTH * 0.5)

		pier = add_box(
			f"Pier_{i+1:02d}",
			location=(center_pt.x, center_pt.y, center_pt.z + pier_height / 2.0),
			size_xyz=(PIER_LENGTH, PIER_WIDTH, pier_height),
		)
		pier.rotation_euler = (0.0, 0.0, a)
		pier.data.materials.append(mat_pier)
		link_only(pier, col)
		created.append(pier)


	# Camera (top-down ortho)
	if ADD_CAMERA:
		cam_data = bpy.data.cameras.new("Camera")
		cam_data.type = "ORTHO"
		cam_data.ortho_scale = ORTHO_SCALE
		cam = bpy.data.objects.new("Camera", cam_data)
		bpy.context.collection.objects.link(cam)
		cam.location = (0.0, 0.0, CAMERA_Z)
		cam.rotation_euler = (math.radians(90.0), 0.0, 0.0)
		bpy.context.scene.camera = cam
		link_only(cam, col)

	# Simple lighting
	light_data = bpy.data.lights.new("SunPanel", type="AREA")
	light_data.energy = 5500
	light = bpy.data.objects.new("SunPanel", light_data)
	bpy.context.collection.objects.link(light)
	light.location = (0.0, 0.0, 140.0)
	light.rotation_euler = (0.0, 0.0, 0.0)
	link_only(light, col)

	# Viewport shading best-effort
	try:
		for area in bpy.context.screen.areas:
			if area.type != "VIEW_3D":
				continue
			for space in area.spaces:
				if space.type == "VIEW_3D":
					space.shading.type = "MATERIAL"
	except Exception:
		pass


build()
print("Done: Marina3D generated.")

