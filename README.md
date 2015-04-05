# ニコニコのユーザセッションだけを取得するツールになりたい

いまのところ、適当です。

**まったくもって、まだできていません。**

目標は全部？

指針:

* 共通部分はVSでもコンパイルできるように、2015正式出たら、そっちで。
* Win部分も共通部分とあわす。
* Mac/Linux部分はClangとGCCでコンパイルできればいいので、C11で書く。
* Windowsだけopensslとsqlite3のライブラリをOSで用意してくれない。ほんとうにいけてないOSだな。

OS

* [ ] Windows 8.1
    * [ ] IE11
    * [ ] Firefox
    * [ ] Chrome (暗号化は…)
* [ ] Mac 10.10
    * [x] Safari
    * [x] Firefox
    * [x] Chrome (暗号化は…)
* [ ] Ubuntu 14.04
    * [ ] Firefox
    * [ ] Chrome (暗号化は…)

Operaは要望があったらやる。他のブラウザはプルリク来たら、マージする。

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
C11を前提とする。ただし、Windowsでもコンパイルする部分はVisual Studio 2015(2013じゃなくてね)でコンパイルできるようにする。

## 制限事項

* クッキーが二つ以上ある場合は一番目しか返せない。
* Firefoxのプロファイルが二つ以上ある場合は一番目しか取得しようとしない。
