# 103 计算机纸带转换器

## 主要功能

- 将103机程序代码转换为用于“手动输入”的纸带（包括二进制文件和纸带图形）。
## 使用方法
```bash
./tape-translator <program_file> -o <tape_file>
```
运行后，程序会解析 `<program_file>` 中的代码，将二进制格式的纸带输出到 `<tape_file>` 中，并在终端中绘制纸带图样。例如：
```bash
./tape-translator sqrt.103 -o sprt-tape.bin
```
将 `sqrt.103` 文件中的代码，转换为纸带，输出到 `sqrt-tape.bin` 中。
## 构建
```bash
make all
```
## TODO List

- [ ] 读取一行的方式需要改造，以适应任意长度的注释
- [ ] 由运行选项控制是否输出纸带图样
- [ ] 修改仓库名，更改仓库归属
