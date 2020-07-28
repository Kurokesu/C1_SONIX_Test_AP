Set Sonix chipset based USB camera extension unit parameters (like h.264 compression rate, measurement mode, GPIO, ...)

# Compile


```
cd C1_SONIX_Test_AP
cp Makefile.x86 Makefile
make
```

# Using TestAP

## Print help
```
./SONiX_UVC_TestAP -h
```

## Save MJPG frames (/dev/video0 is MJPG interface)
```
./SONiX_UVC_TestAP /dev/video0 -c -f mjpg -S
```

## Save H.264 video data (/dev/video1 is H.264 interface, need SONiX UVC Like Driver support)
```
./SONiX_UVC_TestAP /dev/video1 -c -f H264 -r
```

## Extension Unit (XU) controls

### Add XU ctrls to uvc driver (if uvc driver doesn't support)
```
./SONiX_UVC_TestAP /dev/video1 -a
```

### Get & Set H.264 resolutions & framerates (1280x720, 24fps), Insure getting format before setting format !
```
./SONiX_UVC_TestAP /dev/video1 --xuget-fmt --xuset-fmt 1-1
```

### Get & Set H.264 QP/Bitrates(Kbps)
```
./SONiX_UVC_TestAP /dev/video1 --xuget-qp --xuset-qp 31 --xuget-br --xuset-br 6882

```
