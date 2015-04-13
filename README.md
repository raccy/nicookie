# ニコニコのユーザセッションだけを取得するツールになりたかった

[Viqo](https://github.com/diginatu/Viqo)に組み込むために、独立して作成中。
将来はViqoに入るので、Qt版はなくなるっすよ。

C版は・・・やるとは言っていない。

## 一応できているところ
* Win
    * IE
        * 保護モード(あやしいし、VS2013W64じゃないと駄目だった)
        * 非保護モード(実は試してない)
    * Firefox
    * Chrome (暗号化対応)
* Mac
    * Safari
    * Firefox
    * Chrome (暗号化対応)
* Linux(動くそうです)
    * Firefox
    * Chrome (暗号化対応)

## できていないところ
* IEで32bitと64bitで見るところが違うかもしれないこと
* Mingw32でのコンパイル(iepmapi関係のヘッダが無いけどどうしよう)

## できていないところ(やるとは言ってない)
* ChromeベースOpera(Chromeと同じっぽいが、復号の鍵が不明)
* 旧Opera(対応しなくてもいいよね？)
* Android(機会があれば)
* iOS(えと、Viqoがたい・・)
* 他のブラウザ(プルリク募集中)

Winは64bitのVS2013でしかうまくいかないです。
32bitのVS2013だとIEのクッキーなぜか取れないし、mingwはIE関係のヘッダが無いし、
IEは捨てていいっすか？
