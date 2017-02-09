var resize = require('bindings')('resize');

module.exports.initSync = function(width, height, destwidth, destheight, depth) {
  return resize.init(width, height, destwidth, destheight, depth);
}


module.exports.resizeSync = function(buffer) {
    return resize.resize(buffer);

};

