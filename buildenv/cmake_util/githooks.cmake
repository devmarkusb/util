# include in your project root once; enables dir .githooks in project root as a place for git hooks under version
# control, opposed to .git/hooks

# in order to work for submodules also I found no other way than to use --global
execute_process(COMMAND git config --global core.hookspath .githooks)
