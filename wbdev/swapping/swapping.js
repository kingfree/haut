function styleExamples() {
  $("div.examples").each(function() {
    createTabs(this);
    activateTabs(this);
    displayTab($(this).children("ul.tabs").children().first());
  });
}

function createTabs(container) {
  $(container).prepend("<ul class='tabs'></ul>");
  $(container).children("div.example").each(function() {
    var exampleTitle = $(this).attr('class').replace('example','');
    $(container).children("ul.tabs").append(
      "<li class='tab "+exampleTitle+"'>"+exampleTitle+"</li>"
    );
  });
}

function displayTab(element) {
  tabTitle = $(element)
    .attr('class')
    .replace('tab','')
    .replace('selected','').trim();

  container = $(element).parent().parent();
  container.children("div.example").hide();
  container.children("ul.tabs").children("li").removeClass("selected");

  container.children("div."+tabTitle).slideDown('fast');
  $(element).addClass("selected");
}

function activateTabs(element) {
  $(element).children("ul.tabs").children("li").click(function() {
    displayTab(this);
  });
}

$(function() {
  styleExamples();
});
