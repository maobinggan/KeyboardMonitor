# KeyboardMonitor
尝试监听密码框，发现密码框会不断地Hook、UnHook键盘钩子，从而使得监听器不能得到完整密码；  
每当密码框先获取到键盘消息时，会把真实按键映射为另一个按键，然后模拟键盘发送这个转换后的假消息；  
同时还会不断发送随机键盘消息，但这种消息可以通过消息的时间戳辨别；  
