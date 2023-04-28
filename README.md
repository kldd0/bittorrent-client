# **General info and auxiliary notes about Torrent protocol and how it works**

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



### Example
```
((dictionary
    (#"announce"      . #"http://cdimage.debian.org:6969/announce")
    (#"comment"       . #"Debian CD from cdimage.debian.org")
    (#"creation date" . 1105009474)
    (#"info"
     dictionary
     (#"length"       . 600158208)
     (#"name"         . #"debian-30r4-i386-binary-1.iso")
     (#"piece length" . 524288)
     (#"pieces"       . [...large byte string...]))))

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
