# Build Troubleshooting Guide

## Error: "cannot open file '...\sis_assigment.exe'"

This error typically occurs when:

### Solution 1: Close Running Application
The most common cause is that the executable is still running:
1. Press `Ctrl+Shift+Esc` to open Task Manager
2. Look for `sis_assigment.exe` in the Processes tab
3. Right-click and select "End Task"
4. Try building again

### Solution 2: Clean Solution
1. In Visual Studio, go to **Build** → **Clean Solution**
2. Wait for the clean to complete
3. Then **Build** → **Rebuild Solution**

### Solution 3: Delete Build Output Manually
If the above doesn't work:
1. Close Visual Studio
2. Navigate to: `x64\Debug\`
3. Delete the `sis_assigment.exe` file if it exists
4. Also delete `sis_assigment.pdb` if present
5. Reopen Visual Studio and rebuild

### Solution 4: Check for Build Errors
1. Look in the **Output** window (View → Output)
2. Check for any compilation errors
3. Fix any errors before trying to run

### Solution 5: Administrator Rights
Sometimes you need to run Visual Studio as Administrator:
1. Right-click Visual Studio shortcut
2. Select "Run as administrator"
3. Try building again

### Solution 6: Antivirus Interference
Antivirus software can sometimes lock executable files:
1. Temporarily disable antivirus
2. Clean and rebuild
3. Re-enable antivirus

## Quick Fix Commands

If using command line:
```batch
taskkill /F /IM sis_assigment.exe
del x64\Debug\sis_assigment.exe
del x64\Debug\sis_assigment.pdb
```

Then rebuild from Visual Studio.

