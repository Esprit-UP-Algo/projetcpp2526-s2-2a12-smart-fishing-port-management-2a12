# Questions & Answers System - UI Update

## Summary of Changes

The Q&A system interface has been updated to replace the drop-down selector (ComboBox) with a more user-friendly text input field (QLineEdit) that accepts question numbers from 1-10.

## What Changed

### User Interface
- **Before**: Dropdown menu to select question number (1-10)
- **After**: 
  - Display of 5 visible sample questions as reference
  - Text input field for user to type question number
  - Input validation for numbers 1-10 only
  - Error messages for invalid input

### New Features
1. **Visible Question List**: First 5 questions displayed with titles for user reference
2. **Text Input Field**: Users can type any number 1-10 directly
3. **Input Validation**: 
   - Ensures only numeric input between 1-10
   - Shows error messages for:
     - Empty input
     - Non-numeric characters
     - Numbers outside 1-10 range
4. **Enhanced UX**:
   - Auto-focus on input field
   - Auto-clear after successful query
   - Error highlighting with warning emoji
   - Responsive styling with dark theme

## Files Modified

1. **mainwindow.h** (Line 99)
   - Changed member variable from `QComboBox *m_questionNumberCombo` to `QLineEdit *m_questionInputField`

2. **mainwindow.cpp**
   - Added `#include <QLineEdit>` (Line 40)
   - Updated `setupQuestionsAnswersUI()` function (Lines 1902-2087)
     - Creates question list display
     - Creates QLineEdit for number input
     - Styles all elements with dark theme
   - Updated `onSendQuestion()` function (Lines 2132-2205)
     - Validates numeric input
     - Provides error feedback
     - Auto-focuses and clears input

## How to Use

1. **Enter Question Number**: Type a number between 1-10 in the input field
2. **Click "Envoyer"**: Click the Send button to display the answer
3. **View Answer**: The answer displays below with formatting
4. **Try Another**: The input field auto-clears for the next question

## Compilation

### Option 1: Using Qt Creator (Recommended)
1. Open Qt Creator
2. File → Open File or Project
3. Select: `C:\Users\chebbi\Desktop\transaction\CMakeLists.txt`
4. Click "Open" and configure project
5. Press **Ctrl+B** to build
6. Press **Ctrl+R** to run

### Option 2: Using Build Script (Windows)
```cmd
C:\Users\chebbi\Desktop\transaction\build_project.bat
```

### Option 3: Command Line (if tools available)
```cmd
cd C:\Users\chebbi\Desktop\transaction\build\Desktop_Qt_6_7_3_MinGW_64_bit-Debug
ninja.exe
transaction.exe
```

## Testing Checklist

- [ ] Application starts successfully
- [ ] 5 questions visible in the display area
- [ ] Can type number in input field
- [ ] Valid input (1-10) shows correct answer
- [ ] Empty input shows error message
- [ ] Non-numeric input shows error message
- [ ] Out-of-range numbers (0, 11, etc.) show error message
- [ ] Input field auto-clears after valid query
- [ ] Input field auto-focuses after query
- [ ] Dark theme styling applied correctly

## Questions & Answers Content

The system contains 10 anomaly detection questions:

1. Qu'est-ce qu'une anomalie de transaction?
2. Comment le système détecte-t-il les anomalies?
3. Quels sont les types d'anomalies?
4. Comment corriger une transaction anormale détectée?
5. Quels seuils sont utilisés pour détecter les anomalies?
6-10. (Additional specialized questions)

Users can query any number from 1-10 to get the corresponding answer.

## Troubleshooting

**Issue**: Application won't compile
- **Solution**: Ensure Qt 6.7.3+ and MinGW 64-bit are installed via Qt Creator

**Issue**: Input field not responding
- **Solution**: Make sure to click "Envoyer" button after entering number

**Issue**: CMake not found error
- **Solution**: Use Qt Creator's built-in build system instead (Recommended)

## Technical Details

### Dark Theme Colors
- Background: `#0d1b2a`
- Border/Text: `#00bcd4` (Cyan)
- Hover: `#00e5ff` (Light Cyan)
- Error: `#ff6b6b` (Red)
- Response Background: `#ecf0f1`

### Input Validation
- Trims whitespace automatically
- Converts to integer and validates range
- Provides immediate feedback
- SelectAll() on invalid numeric input

### Response Formatting
- HTML formatted display
- Question title with emoji
- Horizontal divider
- Colored background for answer text
- Left border in cyan accent

---

**Last Updated**: 2026-01-28  
**Status**: Ready for Compilation and Testing
