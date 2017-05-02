Pod::Spec.new do |s|
  s.name             = 'IndCalculator'
  s.version          = '0.1.0'
  s.summary          = 'IndCalculator'

  s.description      = <<-DESC
    C++计算
    OC桥接, 以及数据协议
                       DESC

  s.homepage         = 'http://ph.benemind.com/diffusion/95'
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { 'aelam' => 'wanglun02@gmail.com' }
  s.source           = { :git => 'http://ph.benemind.com/diffusion/95/indcalculator.git', :tag => s.version.to_s }

  s.ios.deployment_target = '7.0'
#s.xcconfig = {'CLANG_CXX_LANGUAGE_STANDARD' => 'c++14' }

  s.subspec 'Ind' do |ss|
      ss.source_files = 'IndCalculator/Ind/**/*.{h,m,cpp}'
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
