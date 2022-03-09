@echo off
set /p message= "Message for the push: "
git add *
git commit -m %message%
git push origin main

PAUSE