#cc2uml
##简介
cc2uml是我在读tv的源代码的时候，即兴写的一个小程序。之所以写它，是因为好用的UML工具都是java做的，对于我这个Ｃ++程序员来说，java太笨重。而且，好用的都是收费的。

cc2uml最核心的部分是Ｃ++的语法分析和texlive中的pgf-umlcd库。cc2uml目前只能画UML类图，画在pdf文件中。

当然cc2uml只能分析tv的源代码。当cc2uml分析完tv的源代码之后，生成一个.tex文件，这个文件中包含有tv库中所有的类的UML类图。类关系是用pgf-umlcd提供的latex宏描述的。这个.tex文件中的类有很多，在读源代码的时候要一个个类读，就可以把这个.tex文件中的类复制到frame.tex中，复制的时候要把类的父类也复制到frame.tex中，可以不复制类的子类，然后就可以使用xelatex编译frame.tex得到pdf文件，用pdf阅读器查看生成的pdf文件就可以了。

##使用指南
1. 给texlive（仅在texlive 2014和texlive 2015两个版上本测试过）中的pgf-umlcd.sty打补丁。库里的pgf-umlcd.sty已经打好补定了。解决了多继承的问题，并做了一些增强。以windows系统为例：
  1. `move <path-to-texlive>\texlive\texmf-dist\tex\latex\pgf-umlcd\pgf-umlcd.sty <path-to-texlive>\texlive\texmf-dist\tex\latex\pgf-umlcd\pgf-umlcd.sty.orig`
  2. `copy pgf-umlcd_new.sty <path-to-texlive>\texlive\texmf-dist\tex\latex\pgf-umlcd\pgf-umlcd.sty`
  3. `texhash`
2. 编译cc2uml，要用`gcc`编译器，直接在源代码目录里运行`make`（Linux）或`make -f Makefile.win`（windows）就可以了；
3. 解压rhtvision_2.2.1-1.tar.gz；
4. 运行cc2uml.exe，根据提示操作，假设生成的文件名是abcd.tex；
5. 把abcd.tex中的几个有继承关系的类复制到frame.tex中；
6. 用xelatex编译frame.tex
`xelatex frame.tex`
7. [texlive主页](https://www.tug.org/texlive/)
8. [下载texlive](https://www.tug.org/texlive/acquire-iso.html)
