@echo off
set /p message= "(surround message with ") Message for the push: "
git add *
git commit -m %message%
git push origin main

PAUSE