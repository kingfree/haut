$(document).ready(function() {
  $('#getweibo').submit(function(e) {
    var config = {
      source: $('#appkey').val(),
      access_token: $('#appsecret').val(),
      // uid: ,
      // screen_name: ,
      // since_id: ,
      // max_id: ,
      count: 100,
      page: 1,
      // base_app: ,
      // feature: ,
      trim_user: 1,
    };
    var weibo = $.getJSON("https://api.weibo.com/2/statuses/user_timeline.json", config);
    $.each(weibo, function(i, t) {
      $('weibo').append("<tr><td>" + t['text'] +"</td><td>" + t['created_at'] + "</td></tr>");
    });
    e.preventDefault();
  });
});