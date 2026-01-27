#!/usr/bin/env bash
ctags -o - --kinds-C=f -x --_xformat="%{typeref} %{name}%{signature};" "$1" | awk '{sub(/^typename:/, ""); print $0}'
