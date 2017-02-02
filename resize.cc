#include <nan.h>
#include <iostream>

using namespace v8;
using v8::FunctionTemplate;


NAN_METHOD(resize) 
{

  if(info.Length() < 5) 
  {

    return Nan::ThrowError(Nan::TypeError("Too few arguments"));

  }
  int width = info[1]->NumberValue();
  int height = info[2]->NumberValue();
  int desWidth = info[3]->NumberValue();
  int desHeight = info[4]->NumberValue();
  Local<Object> srcBitmap = info[0].As<Object>();
  if(!node::Buffer::HasInstance(srcBitmap))
  {
    return Nan::ThrowError(Nan::TypeError("Invalid source buffer"));
  }


  unsigned char* srcBitmapData = (unsigned char*) node::Buffer::Data(srcBitmap);
  
  Local<Object> resizedBitmap;
  int desBitmapDataSize = desWidth*desHeight*4;
  unsigned char *destBitmapData = (unsigned char*) malloc(sizeof(unsigned char) * desBitmapDataSize); //TODO only for 32bit per pixel

  double w_ratio =  (double) width/(double) desWidth;
  double y_ratio =  (double) height/(double) desHeight;

  for(int i=0; i< desHeight ; i++){
    for(int j = 0; j < desWidth; j++) {
      int px = (int) j*w_ratio;
      int py = (int) i*y_ratio;

     *(destBitmapData + ((i*desWidth)+j)*4) = *(srcBitmapData + ((py*width)+px)*4);
     *(destBitmapData + ((i*desWidth)+j)*4+1) = *(srcBitmapData + ((py*width)+px)*4+1);
     *(destBitmapData + ((i*desWidth)+j)*4+2) = *(srcBitmapData + ((py*width)+px)*4+2);
     *(destBitmapData + ((i*desWidth)+j)*4+3) = *(srcBitmapData + ((py*width)+px)*4+3);
    }
  }

  resizedBitmap = Nan::CopyBuffer((char *) destBitmapData, desBitmapDataSize).ToLocalChecked();
  info.GetReturnValue().Set(resizedBitmap);
  
}

NAN_MODULE_INIT(Init)
{
  Nan::Set(target, Nan::New("resize").ToLocalChecked(),
      Nan::GetFunction(Nan::New<FunctionTemplate>(resize)).ToLocalChecked());

}

NODE_MODULE(resize, Init)
