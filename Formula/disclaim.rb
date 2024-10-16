class Disclaim < Formula
  desc "A macOS command-line tool for launching processes with their own responsibility for TCC permissions"
  homepage "https://github.com/torarnv/disclaim/"
  url "https://github.com/torarnv/disclaim/archive/refs/tags/v0.0.1.zip"
  version "0.0.1"
  sha256 "24363ebda6ee5ef8040fee4ff22ddfa37dead63eb0c1cbcdc4acb9cf2610c4ec"
  license "BSD-2-Clause"

  def install
    system "make"
    bin.install "disclaim"
  end
end
