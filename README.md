# **General info and auxiliary notes about Torrent protocol and how it works**

### Project tree
```tree
bencode/
├── CMakeLists.txt
└── include/
    ├── bencode/
    │   ├── items/
    │   │   ├── Item.hpp
    │   │   ├── List.hpp
    │   │   ├── Dict.hpp
    │   │
    │   ├── figures/
    │   │   ├── models/
    │   │   │   ├── figure.hpp
    │   │   │   ├── point.hpp
    │   │   │   ├── polygon.hpp
    │   │   │   └── polyline.hpp
    │   │   ├── point.hpp
    │   │   ├── polygon.hpp
    │   │   └── polyline.hpp
    │   └── space/
    │       ├── coordinates.hpp
    │       ├── space.hpp
    │       └── transform.hpp
    └── bencode.hpp
```

### Ubuntu file
```
d
  8:announce
    35:https://torrent.ubuntu.com/announce
    13:announce-list
    l
      l
      35:https://torrent.ubuntu.com/announce
      e
      l
      40:https://ipv6.torrent.ubuntu.com/announce
      e
    e
  7:comment
    29:Ubuntu CD releases.ubuntu.com
    10:created by
    13:mktorrent 1.1
  13:creation date
    i1666283028e
  4:info
  d
    6:length
      i4071903232e 
    4:name
      30:ubuntu-22.10-desktop-amd64.iso
    12:piece length
      i262144e
    6:pieces
      310680:(byte string)^h~G$a# β,>-ϭH
ee
```


### Manjaro file
```
d
  8:announce
    33:udp://tracker.opentrackr.org:1337
  10:created by
    13:mktorrent 1.1
  13:creation date
    i1682071490e
  4:info
    d
      6:length
        i4081973248e
      4:name
        39:manjaro-gnome-22.1.0-230421-linux61.iso
        12:piece length
          i2097152e
        6:pieces
          38940:(byte string)??){
ee
```

### Debian file
```
d
  8:announce
    41:http://bttracker.debian.org:6969/announce
  7:comment
    35:"Debian CD from cdimage.debian.org"
  10:created by
    13:mktorrent 1.1
  13:creation date
    i1671279444e
  4:info
    d
      6:length
        i406847488e
      4:name
        31:debian-11.6.0-amd64-netinst.iso
      12:piece length
        i262144e
      6:pieces
        31040:(byte string)1rYڄ
ee
```

### Atomic Heart
```
d
  8:announce
    42:udp://tracker.opentrackr.org:1337/announce
  13:announce-list
    l
      l
        42:udp://tracker.opentrackr.org:1337/announce
      e
      l
        41:udp://opentracker.i2p.rocks:6969/announce
      e
      l
        42:https://opentracker.i2p.rocks:443/announce
      e
      l
        46:udp://tracker.openbittorrent.com:6969/announce
      e
      l
        45:http://tracker.openbittorrent.com:80/announce
      e
      l
        31:udp://9.rarbg.com:2810/announce
      e
      l
        36:udp://open.demonii.com:1337/announce
      e
      l
        37:udp://exodus.desync.com:6969/announce
      e
      l
        33:udp://open.stealth.si:80/announce
      e
      l
        41:udp://tracker.torrent.eu.org:451/announce
      e
      l
        32:udp://retracker.megaseed.kz:6969
      e
    e
  10:created by
  31:★★★ megaseed.kz ★★★
  13:creation date
    i1682391412e
  4:info
    d
      5:files
      l
        d
          6:length
            i74553512e
          4:path
          l
            9:setup.exe
          e
        e
        d
          6:length
            i30399772226e
          4:path
          l
            4:data
            8:data.bin
          e
        e
        d
          6:length
            i3087738e
          4:path
          l
            4:data
            9:setup.exe
          e
        e
      e
      4:name
        22:Atomic Heart by Igruha
      12:piece length
        i33554432e
      6:pieces
        18180:z?H???d?n??#??̀?????P4?y??(????????px?v?("
```



### Example
```
((dictionary
    (#"announce"      . #"http://cdimage.debian.org:6969/announce")
    (#"comment"       . #"Debian CD from cdimage.debian.org")
    (#"creation date" . 1105009474)
    (#"info"
     dictionary
     --- when multiple files are being shared
     (#"files [a list of dictionaries]")
      [
        (#"length" length—size of the file in bytes.
        (#"path" path—a list of strings corresponding to subdirectory names, the last of which is the actual file name
      ]
     --- when one file is being shared 
     (#"length"       . 600158208)
     (#"name"         . #"debian-30r4-i386-binary-1.iso")
     (#"piece length" . 524288)
     (#"pieces"       . [...large byte string...] [it's a hash list]))))

d['announce-list'] = [ [tracker1], [backup1], [backup2] ]
d['announce-list'] = [[ tracker1, tracker2, tracker3 ]]
```

### Some info from Wiki
```
1) Strings: coded as a decimal number giving the length of the string, then a colon, then the string itself; e.g., 5:stuff
2) Integers: coded as the letter "i", then the integer (as a series of decimal digits), then the letter "e"' e.g., i42e
3) Lists: coded as the letter "l", then the list elements (each encoded as one of the Bencode types,
   which can include other lists), then the letter "e"; e.g.,
   li42e5:stuffi666ee (which contains 42, 'stuff', and 666 as its elements)
4) Dictionaries: pairs of keys and values, where the key is a string and the value can be any Bencode type;
   surrounded by an opening letter 'd' and closing letter 'e'; keys must appear in alphabetic order;
   e.g., d4:testi42e3:zzz4:junke is a dictionary where key 'test' has value 42 and key 'zzz' has value 'junk'.
```

### Example of GET request to tracker
```
http://some.tracker.com:999/announce
?info_hash=12345678901234567890
&peer_id=ABCDEFGHIJKLMNOPQRST
&ip=255.255.255.255
&port=6881
&downloaded=1234
&left=98765
&event=stopped
```
