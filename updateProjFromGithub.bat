
@echo off
set /p httpsAddres=Addres to an already existing repository?:
@echo URL='%httpsAddres%'

set /p message=Message for push?:
@echo URL='%message%'

git pull %httpsAddres%
git add *
git commit -m "%message%"
git remote add origin %httpsAddres%
git push -u origin main