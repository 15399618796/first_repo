#配置提示符的颜色
PROMPT="%(?:%{$fg_bold[green]%}➜ :%{$fg_bold[red]%}➜ )%{$fg[cyan]%}%~%{$reset_color%}" #这里的%~ 表示用~代表家目录

PROMPT+=' $(git_prompt_info)'  #显示gi相关t信息
ZSH_THEME_GIT_PROMPT_PREFIX="%{$fg_bold[blue]%}git:(%{$fg[red]%}" #定义了 Git 仓库状态的前缀部分
ZSH_THEME_GIT_PROMPT_SUFFIX="%{$reset_color%}" #定义了 Git 仓库状态的后缀部分
ZSH_THEME_GIT_PROMPT_DIRTY="%{$fg[blue]%})%{$fg[yellow]%}%1{✗ %}" #定义了 Git 仓库处于未提交状态时的显示方式
ZSH_THEME_GIT_PROMPT_CLEAN="%{$fg[blue]%})" #定义了 Git 仓库处于干净状态时的显示方式

