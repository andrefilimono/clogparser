# clogparser

`clogparser` parses _Icecast_ access log entries into python list.

Tested with:
- python 2.6.9
- python 2.7.13
- python 3.6.1

## Usage
-----

```bash
python setup.py install
```

```python
  import clogparser

  log_entry = '127.0.0.1 - - [25/Jul/2017:03:15:22 +0300] "GET /client_name/stream.mp3 HTTP/1.0" 200 66520 "-" "Winamp 2.81" 0'
  result = clogparser.parse_icecast(log_entry)

  """
  result will be:
  ['127.0.0.1', '-', '-', '25/Jul/2017:03:15:22 +0300', 'GET /client_name/stream.mp3 HTTP/1.0', '200', '66520', '-', 'Winamp 2.81', '0']
  """
```

If parsing fails, `None` will be returned. The parser will attempt to parse as
much of the data as possible.
