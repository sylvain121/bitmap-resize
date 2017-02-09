#include <nan.h>
#include <iostream>

using namespace v8;
using v8::FunctionTemplate;


int width = 0;
int height = 0;
int desWidth = 0;
int desHeight = 0;

int depth = 0;

unsigned char *destBitmapData = NULL;
int desBitmapDataSize = 0;
double w_ratio;
double y_ratio;

NAN_METHOD(init) {
  if(info.Length() < 5) 
  {

    return Nan::ThrowError(Nan::TypeError("Too few arguments"));

  }
  width = info[0]->NumberValue();
  if(width < 0) return Nan::ThrowError(Nan::TypeError("width must be greatter than 0"));
  height = info[1]->NumberValue();
  if(height < 0) return Nan::ThrowError(Nan::TypeError("height must be greatter than 0"));
  desWidth = info[2]->NumberValue();
  if(desWidth < 0) return Nan::ThrowError(Nan::TypeError("destination width must be greatter than 0"));
  desHeight = info[3]->NumberValue();
  if(desHeight < 0) return Nan::ThrowError(Nan::TypeError("destination height  must be greatter than 0"));
  int depth_in_bit = info[4]->NumberValue();
  if(depth_in_bit == 32) depth = 4;
  if(depth_in_bit == 24) depth = 3;
  if(depth_in_bit == 16) depth = 2;
  if(depth_in_bit == 8) depth = 1;
  if(depth < 0) return Nan::ThrowError(Nan::TypeError("depth  must be greatter than 0"));

  desBitmapDataSize = desWidth*desHeight*depth;
  destBitmapData = (unsigned char*) malloc(sizeof(unsigned char) * desBitmapDataSize);

  w_ratio =  (double) width/(double) desWidth;
  y_ratio =  (double) height/(double) desHeight;

}


NAN_METHOD(resize) 
{

  if(info.Length() < 1) 
  {

    return Nan::ThrowError(Nan::TypeError("Too few arguments"));

  }
  Local<Object> srcBitmap = info[0].As<Object>();
  if(!node::Buffer::HasInstance(srcBitmap))
  {
    return Nan::ThrowError(Nan::TypeError("Invalid source buffer"));
  }


  unsigned char* srcBitmapData = (unsigned char*) node::Buffer::Data(srcBitmap);


  for(int i=0; i< desHeight ; i++){
    for(int j = 0; j < desWidth; j++) {
      int px = (int) j*w_ratio;
      int py = (int) i*y_ratio;

      *(destBitmapData + ((i*desWidth)+j)*4) = *(srcBitmapData + ((py*width)+px)*4);
      if(depth >= 2) *(destBitmapData + ((i*desWidth)+j)*depth+1) = *(srcBitmapData + ((py*width)+px)*depth+1);
      if(depth >= 3) *(destBitmapData + ((i*desWidth)+j)*depth+2) = *(srcBitmapData + ((py*width)+px)*depth+2);
      if(depth >= 4) *(destBitmapData + ((i*desWidth)+j)*depth+3) = *(srcBitmapData + ((py*width)+px)*depth+3);
    }
  }

  info.GetReturnValue().Set(Nan::CopyBuffer((char *) destBitmapData, desBitmapDataSize).ToLocalChecked());

}

NAN_MODULE_INIT(Init)
{
  Nan::Set(target, Nan::New("resize").ToLocalChecked(),
      Nan::GetFunction(Nan::New<FunctionTemplate>(resize)).ToLocalChecked());
  Nan::Set(target, Nan::New("init").ToLocalChecked(),
      Nan::GetFunction(Nan::New<FunctionTemplate>(init)).ToLocalChecked());

}

NODE_MODULE(resize, Init)
