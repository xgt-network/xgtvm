def which?(cmd_name)
  %x([ -x "$(command -v #{cmd_name})" ] && echo yes).length > 0
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
task :run do
  sh %(cd build && ./xgtvm#{ which?('jq') ? '| jq .' : '' })
end

task :default => [:configure, :make]
