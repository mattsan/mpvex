defmodule Mpvex.MixProject do
  use Mix.Project

  def project do
    [
      app: :mpvex,
      version: "0.1.0",
      elixir: "~> 1.11",
      start_permanent: Mix.env() == :prod,
      deps: deps(),
      compilers: compilers(Mix.env()),
      package: package()
    ]
  end

  def application do
    [
      extra_applications: [:logger]
    ]
  end

  defp deps do
    [
      {:elixir_make, "~> 0.6", runtime: false},
      {:ex_doc, "~> 0.23", runtime: false, only: :dev},
      {:dialyxir, "~> 1.0", runtime: false, only: :dev}
    ]
  end

  defp compilers(:test), do: Mix.compilers()
  defp compilers(_), do: [:elixir_make | Mix.compilers()]

  defp package do
    %{
      files: [
        "lib",
        "src/*.cpp",
        "src/*.h",
        "mix.exs",
        "README.md",
        "Makefile"
      ]
    }
  end
end
