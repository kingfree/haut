$(function() {
  $('#slideshow').cycle({fx: 'zoom'});
  setupButtons();
});

var setupButtons = function() {
  var slideShow = $('#slideshow');

  var pause = $('<span id="pause">暂停</span>');
  pause.click(function() { 
    slideShow.cycle('pause');
    toggleControls();
  }).insertAfter(slideShow);
  
  var resume = $('<span id="resume">继续</span>');
  resume.click(function() { 
    slideShow.cycle('resume'); 
    toggleControls();
  }).insertAfter(slideShow);

  resume.toggle();
};

var toggleControls = function() {
  $('#pause').toggle();
  $('#resume').toggle();
};
