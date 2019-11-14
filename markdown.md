# 语法说明

https://github.com/riku/Markdown-Syntax-CN/blob/master/syntax.md
https://help.github.com/articles/github-flavored-markdown

# pandoc

## mac install

brew install pandoc
brew cask install mactex

## 使用

https://pandoc.org/getting-started.html
https://pandoc.org/demos.html
https://pandoc.org/MANUAL.html

pandoc test.md  -f markdown -t html -s --metadata pagetitle="test" -o test1.html

pandoc test.md -o test.pdf --pdf-engine=xelatex  -V mainfont='STHeiti'

## slides

https://www.jianshu.com/p/6e2ec0b8fff1
https://github.com/hakimel/reveal.js#theming
https://stackoverflow.com/questions/30988306/level-1-and-level-2-slides-in-reveal-js-using-pandoc
左对齐(未测试)
https://stackoverflow.com/questions/21019476/how-can-i-get-left-justified-paragraphs-in-reveal-js

```shell
git clone https://github.com/hakimel/reveal.js
pandoc slides.md -o slides.html -t revealjs -s
pandoc slides.md -o slides.html -t revealjs -s --metadata pagetitle="slides" --slide-level 2 -V theme=white
```
主题:
* black: Black background, white text, blue links (default theme)
* white: White background, black text, blue links
* league: Gray background, white text, blue links (default theme for reveal.js < 3.0.0)
* beige: Beige background, dark text, brown links
* sky: Blue background, thin dark text, blue links
* night: Black background, thick white text, orange links
* serif: Cappuccino background, gray text, brown links
* simple: White background, black text, blue links
* solarized: Cream-colored background, dark green text, blue links