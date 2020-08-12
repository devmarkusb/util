# include in your project root once; enables dir .githooks in project root as a place for git hooks under version
# control, opposed to .git/hooks

execute_process(COMMAND git config core.hookspath .githooks)
