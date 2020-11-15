defmodule Mpvex.NIF do
  @moduledoc false

  @on_load {:load_nif, 0}
  @compile {:autoload, false}

  def load_nif do
    Application.app_dir(:mpvex, "priv/mpvex_nif")
    |> to_charlist()
    |> :erlang.load_nif(0)
  end

  def command(_ctx, _commands), do: :erlang.nif_error(:nif_not_loaded)
  def create, do: :erlang.nif_error(:nif_not_loaded)
  def error_string(_error), do: :erlang.nif_error(:nif_not_loaded)
  def event_name(_event), do: :erlang.nif_error(:nif_not_loaded)
  def initialize(_ctx), do: :erlang.nif_error(:nif_not_loaded)
  def set_option(_ctx, _name, _format, _data), do: :erlang.nif_error(:nif_not_loaded)
  def set_option_string(_ctx, _name, _data), do: :erlang.nif_error(:nif_not_loaded)
  def terminate_destroy(_ctx), do: :erlang.nif_error(:nif_not_loaded)
end
