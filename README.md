# Convolutional Recurrent Neural Network

​This software implements the Convolutional Recurrent Neural Network (CRNN) in caffe. Origin software could be found in [crnn](https://github.com/bgshih/crnn) .You can also find the [cnn.pytorch](https://github.com/meijieru/crnn.pytorch) 

# Crnn.caffe

​The initial environment for this project is [warpctc-caffe](https://github.com/xmfbit/warpctc-caffe).
​The network framework is from  [cnn.pytorch](https://github.com/meijieru/crnn.pytorch) ,it also can be read from  [crnn](https://github.com/bgshih/crnn) 

​CNN-LSTM-CTC, CNN can get BxC feature vectors,they are as the input of LSTM,LTSM can work the vertors in time_step.From CRNN you can get better way to work as the end-to-end character recognition.At first,I just use the LSTM-CTC( [warpctc-caffe](https://github.com/xmfbit/warpctc-caffe) ) to work with the license plate recognition.Loss is very slow convergence and it always overfitting.The result don't like the [cnn.pytorch](https://github.com/meijieru/crnn.pytorch) ,I run the crnn.pytorch to work with these license plate.Finally,I am determined to write the crnn.caffe.The caffe environment is easier to make a product.

# Build

clone the code

> git clone git@github.com:yalecyu/crnn.caffe.git
>
> cd crnn.caffe
>
> make

If there some problems,you can confirm your basic the setting of caffe and server.Build this project is the same way like build caffe.

If you are using mac, you need to change something:

1. Makefile.config
```
-# CPU_ONLY := 1
+CPU_ONLY := 1

-# CUSTOM_CXX := g++
+CUSTOM_CXX := g++
```

2. src/caffe/layers/ctc_loss_layer.cpp
```
+#include <numeric>
 #include <caffe/layers/ctc_loss_layer.hpp>
```

# Test
For example,I run a smaill captcha model.You can use like: ./build/examples/cpp_recognition/recognition.bin (your).jpg (your).prototxt (your).caffemodel

> ./build/examples/cpp_recognition/recognition.bin data/captcha/1.png examples/crnn/deploy.prototxt examples/crnn/model/crnn_captcha.caffemodel

![avatar](https://github.com/yalecyu/crnn.caffe/blob/master/data/captcha/1.png)
"- - - - - 8 8 - - - - 0 - - - - 3 3 - - - - 5 - - - - - 9 9 - -"
the ans is 80359
You can change some parts of the examples/crnn/deploy.prototxt & examples/cpp_recognition/recognition.cpp & examples/crnn/crnn.prototxt,to suit your project.

# Train
#### make data
> python examples/crnn/generate_captcha.py
>
> python examples/crnn/generate_dateset.py


* You can read the image at data/captcha/ 

#### train
> ./examples/crnn/train.sh

#### free time
* a cup of caffe
* You can get the more information by reading code

# Acknowledgements

The authors would like to thank the developers of  [crnn](https://github.com/bgshih/crnn)  [crnn.pytorch](https://github.com/meijieru/crnn.pytorch)  [warpctc-caffe](https://github.com/xmfbit/warpctc-caffe),without your help ,I can't write the crnn.caffe.When I send the email to you,always get the suggest quickly.

Please let me know if you encounter any issues.


