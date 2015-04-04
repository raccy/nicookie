# ニコニコのユーザセッションだけを取得するツールになりたい

いまのところ、適当です。

今のところ、OSXのsafariのみ。それ以外は知らん。

目標は全部？

指針:

* 共通部分はVSでもコンパイルできるように、2015正式出たら、そっちで。
* Win部分も共通部分とあわす。
* Mac/Linux部分はClangとGCCでコンパイルできればいいので、C11で書く。

OS

* [ ] Windows 8.1
    * [ ] IE11
    * [ ] Firefox
    * [ ] Chrome (暗号化は…)
* [ ] Mac 10.10
    * [x] Safari
    * [ ] Firefox
    * [ ] Chrome (暗号化は…)
* [ ] Ubuntu 14.04
    * [ ] Firefox
    * [ ] Chrome (暗号化は…)

目指すところ

各言語用ライブラリがつくれたらいいなー

* C
* C++
* Ruby
* Python
* Java
* Scala
* C#

## コーディングについて

コーディングスタイルは[Google C++ Style Guide](https://code.google.com/p/google-styleguide/)に基づく。
C11を前提とする。ただし、Windowsでもコンパイルする部分はVisual Studio 2015でコンパイルできるようにする。
