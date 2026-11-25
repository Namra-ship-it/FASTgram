FASTagram - Object-Oriented Programming Semester Project
=========================================================

Campus Community Desktop Social App
FAST NUCES, Fall 2026

1. Overview:
   FASTagram is a local desktop photo-sharing and campus social application built in C++17
   using raylib for windowing, drawing, input, and font rendering.
   Window size: 480 x 820 px.

2. Architecture & OOP Highlights:
   - UI Layer: Custom Widget hierarchy (Widget -> Button, TextBox) with virtual draw and update.
   - Models Layer: Account hierarchy (Account base, Student, Faculty, Admin, Society).
   - Core & Services Layer: Session (Singleton), UserManager (Authentication, Persistence).
   - Binary Persistence: Serializable interface with binary file streaming for users.dat.
   - Screen State Machine: LoginScreen, SignupScreen, ProfileScreen, SettingsScreen.

3. Features Implemented in this Milestone:
   - A1: Sign up form with roll number validation and duplicate checks.
   - A2: Log in and log out with Session state tracking.
   - A3: Account types (Student, Faculty, Society, Admin).
   - A4: Profile view with stats, avatar, and 3-column photo grid.
   - T1: Light and Dark theme appearance switch.
   - T2: Custom pill buttons with smooth hover animations and I-beam text input.

4. Build Instructions:
   - Open fastgram.sln in Visual Studio 2022 (v143 / v145).
   - Select Configuration: Debug | x64 (or Release | x64).
   - Build Solution (Ctrl + Shift + B).
   - Run Local Windows Debugger (F5).
