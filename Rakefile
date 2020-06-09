task :default do
  sh 'mkdir -p build'
  sh 'cd build && cmake ..'
  sh 'cd build && cmake --build .'
  sh 'cd build && ./xgtvm'
end
