#! /bin/sh
git config --global alias.spull '__git_spull() { git pull "$@" && git submodule sync --recursive && git submodule update --init --recursive; }; __git_spull'

git config --global alias.smerge '__git_smerge() { git merge "$@" && git submodule sync --recursive && git submodule update --init --recursive; }; __git_merge'
