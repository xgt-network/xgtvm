def which?(cmd_name)
  %x([ -x "$(command -v #{cmd_name})" ] && echo yes).length > 0
end

task :default do
  sh %(rm -rf build)
  sh %(mkdir -p build)
  sh %(cd build && cmake ..)
  sh %(cd build && cmake --build .)
  sh %(cd build && ./xgtvm#{ which?('jq') ? '| jq .' : '' })
end
