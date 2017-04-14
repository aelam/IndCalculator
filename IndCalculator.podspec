Pod::Spec.new do |s|
  s.name             = 'IndCalculator'
  s.version          = '0.1.0'
  s.summary          = 'IndCalculator'

  s.description      = <<-DESC
    C++计算
    OC桥接, 以及数据协议
                       DESC

  s.homepage         = 'https://github.com/aelam/IndCalculator'
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { 'aelam' => 'wanglun02@gmail.com' }
  s.source           = { :git => 'https://github.com/aelam/IndCalculator.git', :tag => s.version.to_s }

  s.ios.deployment_target = '7.0'
  s.compiler_flags = "-fmodules, -fcxx-modules"

  s.subspec 'Ind' do |ss|
      ss.source_files = 'IndCalculator/Ind/**/*.{h,m,cpp}'
      ss.xcconfig = {'GCC_INPUT_FILETYPE' => 'sourcecode.cpp.objcpp' }
      ss.private_header_files = 'IndCalculator/Ind/**/*.{h}'
  end

  s.subspec 'IndFactory' do |ss|
      ss.source_files = 'IndCalculator/IndFactory/**/*.{h,m,cpp}'
      ss.private_header_files = 'IndCalculator/IndFactory/**/*.{h}'
      ss.dependency 'IndCalculator/Ind'
  end

  s.subspec 'OCBridge' do |ss|
      ss.subspec 'Interface' do |sss|
        sss.source_files = 'IndCalculator/OCBridge/Interface/**/*.{h,m,mm}'
        sss.dependency 'IndCalculator/IndFactory'
      end

      ss.subspec 'Implementation' do |sss|
        sss.source_files = 'IndCalculator/OCBridge/Implementation/**/*.{h,m,mm}'
        sss.dependency 'IndCalculator/OCBridge/Interface'
      end
  end

end
