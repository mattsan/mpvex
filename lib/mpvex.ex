defmodule Mpvex do
  @moduledoc """
  Implements some mpv client APIs.

  See https://github.com/mpv-player/mpv/blob/master/libmpv/client.h

  ## Example

  Implimentation of a simple example.

  See https://github.com/mpv-player/mpv-examples/blob/master/libmpv/simple/simple.c

  > Very primitive terminal-only example. Shows some most basic API usage.
  >
  > https://github.com/mpv-player/mpv-examples/blob/master/libmpv/README.md#simple

  ```elixir
  {:ok, ctx} = Mpvex.create()
  Mpvex.set_option_string(ctx, "input-default-bindings", "yes")
  Mpvex.set_option_string(ctx, "input-vo-keyboard", "yes")
  Mpvex.set_option(ctx, "osc", :mpv_format_flag, 1)
  Mpvex.initialize(ctx)
  Mpvex.command(ctx, ["loadfile", filename_or_url])

  # ...

  Mpvex.terminate_destroy(ctx)
  """

  alias Mpvex.NIF

  @type mpv_handle :: reference()
  @type mpv_event_id :: integer()
  @type mpv_format ::
          :mpv_format_none
          | :mpv_format_string
          | :mpv_format_osd_string
          | :mpv_format_flag
          | :mpv_format_int64
          | :mpv_format_double
          | :mpv_format_node
          | :mpv_format_node_array
          | :mpv_format_node_map
          | :mpv_format_byte_array
  @type mpv_data :: any()

  @doc """
  Sends a command to type player.
  """
  @spec command(mpv_handle(), [binary()]) :: :ok | {:error, integer()}
  def command(ctx, commands) when is_reference(ctx) and is_list(commands) do
    NIF.command(ctx, commands)
  end

  @doc """
  Creates a new instance.
  """
  @spec create :: {:ok, mpv_handle()} | {:error, atom()}
  def create do
    NIF.create()
  end

  @doc """
  Returns a string describing the error.
  """
  @spec error_string(integer()) :: {:ok, binary()}
  def error_string(error) when is_integer(error) do
    NIF.error_string(error)
  end

  @doc """
  Returns a string describing the event.
  """
  @spec event_name(mpv_event_id()) :: {:ok, binary()}
  def event_name(event) when is_integer(event) do
    NIF.event_name(event)
  end

  @doc """
  Initializes a instance.
  """
  @spec initialize(mpv_handle()) :: :ok | {:error, integer()}
  def initialize(ctx) when is_reference(ctx) do
    NIF.initialize(ctx)
  end

  @doc """
  Sets an option.

  Implements only `:mpv_format_flag` now.
  """
  @spec set_option(mpv_handle(), binary(), mpv_format(), mpv_data()) :: :ok | {:error, integer()}

  def set_option(ctx, name, :mpv_format_none, data) when is_reference(ctx) and is_binary(name) do
    NIF.set_option(ctx, name, 0, data)
  end

  def set_option(ctx, name, :mpv_format_string, data)
      when is_reference(ctx) and is_binary(name) do
    NIF.set_option(ctx, name, 1, data)
  end

  def set_option(ctx, name, :mpv_format_osd_string, data)
      when is_reference(ctx) and is_binary(name) do
    NIF.set_option(ctx, name, 2, data)
  end

  def set_option(ctx, name, :mpv_format_flag, data) when is_reference(ctx) and is_binary(name) do
    NIF.set_option(ctx, name, 3, data)
  end

  def set_option(ctx, name, :mpv_format_int64, data) when is_reference(ctx) and is_binary(name) do
    NIF.set_option(ctx, name, 4, data)
  end

  def set_option(ctx, name, :mpv_format_double, data)
      when is_reference(ctx) and is_binary(name) do
    NIF.set_option(ctx, name, 5, data)
  end

  def set_option(ctx, name, :mpv_format_node, data) when is_reference(ctx) and is_binary(name) do
    NIF.set_option(ctx, name, 6, data)
  end

  def set_option(ctx, name, :mpv_format_node_array, data)
      when is_reference(ctx) and is_binary(name) do
    NIF.set_option(ctx, name, 7, data)
  end

  def set_option(ctx, name, :mpv_format_node_map, data)
      when is_reference(ctx) and is_binary(name) do
    NIF.set_option(ctx, name, 8, data)
  end

  def set_option(ctx, name, :mpv_format_byte_array, data)
      when is_reference(ctx) and is_binary(name) do
    NIF.set_option(ctx, name, 9, data)
  end

  @doc """
  Set an option to a string value.
  """
  @spec set_option_string(mpv_handle(), binary(), binary()) :: :ok | {:error, integer()}
  def set_option_string(ctx, name, data)
      when is_reference(ctx) and is_binary(name) and is_binary(data) do
    NIF.set_option_string(ctx, name, data)
  end

  @doc """
  Terminates and destroys an instance.
  """
  @spec terminate_destroy(mpv_handle()) :: :ok
  def terminate_destroy(ctx) when is_reference(ctx) do
    NIF.terminate_destroy(ctx)
  end
end
