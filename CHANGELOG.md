# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

### Added
add player can now play against cpu
added new void in pong called cpu_ia
add new state in pong called MENU
add new bool variable in pong called CPU
in void update_pong add the cpu ia only if playing against cpu
in void render_pong add state MENU

### Changed
in void init_pong now pong->state starts in MENU
in state PLAY now when playing against cpu the up and down keys are disabled
in state DONE now can restart or go back menu
in pong.c line 107 the reset scores only excecute if the state isnt MENU
