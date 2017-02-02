var resize = require('bindings')('resize');

module.exports.resizeSync = function(image) {
  
  return resize.resize(
    image.data,
    image.width,
    image.height,
    image.destwidth,
    image.destheight);

};

