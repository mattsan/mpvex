defmodule Mpvex.Player do
  @moduledoc """
  Implimentation of a simple example.

  See https://github.com/mpv-player/mpv-examples/blob/master/libmpv/simple/simple.c
  """

  require Logger

  @spec play(binary()) :: {:ok, Mpvex.mpv_handle()} | {:error, integer()}
  def play(source) do
    case Mpvex.create() do
      {:ok, ctx} ->
        with :ok <- Mpvex.set_option_string(ctx, "input-default-bindings", "yes"),
             :ok <- Mpvex.set_option_string(ctx, "input-vo-keyboard", "yes"),
             :ok <- Mpvex.set_option(ctx, "osc", :mpv_format_flag, 1),
             :ok <- Mpvex.initialize(ctx),
             :ok <- Mpvex.command(ctx, ["loadfile", source]) do
          {:ok, ctx}
        else
          error ->
            Mpvex.terminate_destroy(ctx)
            Logger.error(inspect(error))
            error
        end

      error ->
        Logger.error(inspect(error))
        error
    end
  end
end
