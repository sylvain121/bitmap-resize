#include <nan.h>

using namespace v8;

void resize(const Nan::FunctionCallbackInfo<Value>& info) {

  if (info.Length() < 5) {
    //TODO set correct argument
    Nan::ThrowTypeError("Wrong number of arguments");
    return;

  }

  int width = info[1]->NumberValue();
  int height = info[2]->NumberValue();
  int desWidth = info[3]->NumberValue();
  int desHeight = info[4]->NumberValue();
  Local<Object> srcObject = info[0].As<Object>();

  unsigned char* srcBitmap = (unsigned char*) node::Buffer::Data(srcObject);
  unsigned char desBitmap[desWidth*desHeight*4]; 

  double w_ratio = width/desWidth;
  double y_ratio = height/desHeight;

  for(var i=0; i< desHeight ; i++){
    for(var j = 0; j < desWidth; i++) {
      double px = j*x_ratio;
      double py = i*y_ratio;

      desBitmap[((i*desWidth)+j)*4] = srcBitmap[((py*width)+px)*4];
      desBitmap[((i*desWidth)+j)*4+1] = srcBitmap[((py*width)+px)*4+1];
      desBitmap[((i*desWidth)+j)*4+2] = srcBitmap[((py*width)+px)*4+2];
      desBitmap[((i*desWidth)+j)*4+3] = srcBitmap[((py*width)+px)*4+3];
      


    }
  }

  double arg0 = info[0]->NumberValue();
  double arg1 = info[1]->NumberValue();
  v8::Local<v8::Number> num = Nan::New(arg0 + arg1);

  info.GetReturnValue().Set(num);

}

void Init(Local<Object> exports) {
  exports->Set(Nan::New("resize").ToLocalChecked(),
      Nan::New<FunctionTemplate>(resize)->GetFunction());

}

NODE_MODULE(resize, Init)
