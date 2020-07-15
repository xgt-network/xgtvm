require 'json'

def which?(cmd_name)
  return false
  %x([ -x "$(command -v #{cmd_name})" ] && echo yes).length > 0
end

def xgtvm_eval(string)
  cmd = %(cd build && ./xgtvm -e'#{string}')
  $stderr.puts(cmd)
  result = %x(#{cmd})
  JSON.load(result)
end

desc 'removes build artifacts'
task :clean do
  sh %(rm -rf build)
end

desc 'configures the project'
task :configure do
  sh %(mkdir -p build)
  sh %(cd build && cmake ..)
end

desc 'builds the project'
task :make do
  sh %(mkdir -p build)
  sh %(cd build && cmake --build .)
end

desc 'runs the project'
task :test do
  # result = xgtvm_eval('0000000000000060 0000000000000003 0000000000000060 0000000000000008 0000000000000001 0000000000000000')
  result = xgtvm_eval('60 03 60 08 01 00')
  p result
  stack = result.fetch('finalState').fetch('stack')
  raise 'fail' unless stack == ['11']
end

task :default => [:configure, :make]
