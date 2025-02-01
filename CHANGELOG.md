# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

### Added
added player can now play against cpu
added new void called cpu_ia (pong.c in line 143 and in pong.h line 51)
added new state called MENU (pong.h in line 21)
added new bool variable called CPU (pong.h in line 38)
in void update_pong add the cpu_ia only if playing against cpu (pong.c line 172)
in void render_pong add state MENU (pong.c 295)

### Changed
in void init_pong now pong->state starts in MENU (pong.c line 25)
in state PLAY now when playing against cpu the up and down keys are disabled (pong.c line 36)
in state DONE now can restart or go back menu (pong.c line 304)
The reset scores only excecute if the state isnt MENU (pong.c line 107)


### FIXED
winning message vs cpu
erratic moviment cpu paddle
