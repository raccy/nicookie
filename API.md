# API

## nicookie_get_cookie

```
#include <nicookie.h>
char *nicookie_get_cookie(char *buf, size_t size, int browser);
```

### 引数

`buf`はバッファ、`size`はバッファのサイズ(終端含む)、`browser`はブラウザを渡す。取得に成功すれば、`buf`にニコニコ動画ユーザセッションのクッキーの値が入る。

もし、`buf`が`NULL`の場合は`malloc`でメモリ領域を確保して取得する。その場合のサイズは`size`になる。ただし、`size`が`0`の場合はクッキーの大きさ分のサイズになる。

全ての文字列はNULL終端文字列とする。

### 返り値

成功し場合は、クッキーの値が入った文字列を返す。`buf`が`NULL`以外の場合は`buf`自身、`NULL`の場合は`malloc`したポインタになる。`malloc`した値は`free`しなければならない。

### エラー

失敗時は`errno`に次の値をいれる。

*   `ERANGE`
    クッキーの値が`size`の値を越えている。
*   `EINVAL`
    引数が不正。
*   その他
    たぶん、そいつらはIOエラーだと思う。


ERANGE: サイズが越えたとき。
ENOSYS: 実装されていないとき。
ENSIG
