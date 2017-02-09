var resize = require('bindings')('resize');

module.exports.initSync = function(width, height, destwidth, destheight) {
  return resize.init(width, height, destwidth, destheight);
}


module.exports.resizeSync = function(buffer) {
    return resize.resize(buffer);

};

