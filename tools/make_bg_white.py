from PIL import Image, ImageDraw
import sys

src = r"resources/vision_sight.png"
try:
    im = Image.open(src).convert('RGBA')
except Exception as e:
    print('ERROR: could not open', src, e)
    sys.exit(1)

w,h = im.size
cx,cy = w//2, h//2
r = int(min(w,h)/2 * 0.93)

mask = Image.new('L', (w,h), 0)
draw = ImageDraw.Draw(mask)
draw.ellipse((cx-r, cy-r, cx+r, cy+r), fill=255)

bg = Image.new('RGBA', (w,h), (255,255,255,255))
out = Image.composite(im, bg, mask)

# Overwrite original
out.convert('RGB').save(src)
print('Saved', src)
