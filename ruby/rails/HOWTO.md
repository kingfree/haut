Ruby on Rails 安装和配置
========================

1. 到 http://rubyinstaller.org/downloads/ 下载：
   * RubyInstallers
   * DEVELOPMENT KIT
   安装之。

2. 设置 Gem 源：
    $ gem sources --remove https://rubygems.org/
    $ gem sources -a http://ruby.taobao.org/
    $ gem sources -l
    *** CURRENT SOURCES ***

    http://ruby.taobao.org
   确保只有 ruby.taobao.org

3. 安装 DevKit 库：
   解压缩 DevKit 并进入该目录：
    $ ruby dk.rb init
    $ ruby dk.rb install

4. 开始安装 Rails：
    $ gem install rails
   似乎需要一段时间。

