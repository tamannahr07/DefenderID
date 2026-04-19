@echo off
echo.
echo ============================================================
echo   DefenderID Smart Gate Security System
echo   Startup Script (Windows)
echo ============================================================
echo.

REM Set working directory to project root (parent of script location)
cd /d "%~dp0.."

REM Check Node.js
node --version >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Node.js not found!
    echo Please install Node.js from https://nodejs.org/en/download/
    pause
    exit /b 1
)

echo [1/4] Installing backend dependencies...
cd backend
call npm install
cd ..

echo [2/4] Installing frontend dependencies...
cd frontend
call npm install
cd ..

echo [3/4] Seeding database with demo data...
cd backend
call npm run seed
cd ..

echo [4/4] Starting servers...
echo.
echo   Backend:  http://localhost:5000
echo   Frontend: http://localhost:5173
echo.

REM Start backend in new window
start "SmartGate Backend" cmd /k "cd backend && npm run dev"

REM Wait 3 seconds then start frontend
timeout /t 3 /nobreak >nul
start "SmartGate Frontend" cmd /k "cd frontend && npm run dev"

echo.
echo Both servers started!
echo Open http://localhost:5173 in your browser.
echo.
pause
