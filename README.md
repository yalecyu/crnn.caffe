# Convolutional Recurrent Neural Network

​This software implements the Convolutional Recurrent Neural Network (CRNN) in caffe. Origin software could be found in [crnn](https://github.com/bgshih/crnn) .You can also find the [cnn.pytorch](https://github.com/meijieru/crnn.pytorch) 

# Crnn.caffe

​The initial environment for this project is [warpctc-caffe](https://github.com/xmfbit/warpctc-caffe). You can build a software environment from "warpctc-caffe" project.The author in caffe integrated ctc-loss part, and add some layers, making lstm-ctc can be used in caffe.

​The network framework is from  [cnn.pytorch](https://github.com/meijieru/crnn.pytorch) ,it also can be read from  [crnn](https://github.com/bgshih/crnn) .We have the same network.

​CNN-LSTM-CTC, CNN can get BxC feature vectors,they are as the input of LSTM,LTSM can work the vertors in time_step.From CRNN you can get better way to work as the end-to-end character recognition.At first,I just use the LSTM-CTC( [warpctc-caffe](https://github.com/xmfbit/warpctc-caffe) ) to work with the license plate recognition.Loss is very slow convergence and it always overfitting.The result don't like the [cnn.pytorch](https://github.com/meijieru/crnn.pytorch) ,I run the crnn.pytorch to work with these license plate.Finally,I am determined to write the crnn.caffe.The caffe environment is easier to make a product.

# Build

clone the code

> git clone git@github.com:yalecyu/crnn.caffe.git
>
> cd crnn.caffe
>
> make

If there some problems,you can confirm your basic the setting of caffe and server.Build this project is the same way like build caffe.

# Test

I will ...

# Train

will

# Acknowledgements

The authors would like to thank the developers of  [crnn](https://github.com/bgshih/crnn)  [cnn.pytorch](https://github.com/meijieru/crnn.pytorch)  [warpctc-caffe](https://github.com/xmfbit/warpctc-caffe),without your help ,I can't write the crnn.caffe.When I send the email to you,always get the suggest quickly.

Please let me know if you encounter any issues.


