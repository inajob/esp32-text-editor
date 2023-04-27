putstring("hello from lua!")
putstring("hello from lua!")

lines = {""}
row = 1
col = 1
lastRow = getmaxline() - 3

globalKey = 0
globalCode = ""
fontH = 12
fontW = 6

function draw()
  setcursor(0, 0)
  -- redraw all lines
  for i, line in pairs(lines) do
    setcursor(0, (i - 1)*fontH)
    setcolor(0,0,0, 0,0,0);
    fillrect(0, (i - 1)*fontH, 320, fontH)
    setcolor(255,255,255, 0,0,0);
    putstring(line)
  end

  -- draw status line
  setcursor(0, (lastRow-1)*fontH)
  putstring("heap: " .. getfreeheap())
  -- draw status line
  setcursor(0, (lastRow)*fontH)
  putstring("key: ")
  putstring(globalKey)
  putstring(",code of c: ")
  if globalCode then
    putstring("" .. globalCode)
  else 
    putstring("nil")
  end 

  -- draw cursor
  count = 0
  pos = 0
  for p, c in utf8.codes(lines[row]) do
    if count == col - 1 then
      break
    end
    pos = pos + fontW
    if c > 0x7F then
      pos = pos + fontW
    end
    count = count + 1
  end
 
  setcursor(pos, (row - 1)*fontH)
  setcolor(0,0,0, 0,255,0);
  c = getCharAt(lines[row], col - 1)
  if c == "" then
    c = "*"
  end
  putstring(c)
  setcolor(255,255,255, 0,0,0);

end

function getCharAt(s, i)
  count = 0
  for p, c in utf8.codes(s) do
    if count == i then
      return utf8.char(c)
    end
    count = count + 1
  end
  return ""
end

function insertChar(s, c, i)
  newLine = ""
  count = 0
  hit = false
  for p, lc in utf8.codes(s) do
    if count == i - 1 then
      newLine = newLine .. c
      hit = true
    end
    newLine = newLine .. utf8.char(lc)
    count = count + 1
  end
  if not(hit) then
    newLine = newLine .. c
  end
  return newLine
end

function removeChar(s, i)
  newLine = ""
  count = 1
  for p, lc in utf8.codes(s) do
    if count ~= i then
      newLine = newLine .. utf8.char(lc)
    end
    count = count + 1
  end
  return newLine
end


function keydown(key, c, ctrl)
  code = string.byte(c)
  globalKey = key
  globalCode = code
  if code == 13 then
    table.insert(lines, row + 1, "")
    row = row + 1
    col = 1
  elseif key == 41 then -- ESC
    collectgarbage()
  elseif key == 82 then -- Up
    if row > 1 then
      row = row - 1
      if col >= utf8.len(lines[row]) then
        col = utf8.len(lines[row]) + 1
      end
    end
  elseif key == 81 then -- Down
    if row < #lines then
      row = row + 1
      if col >= utf8.len(lines[row]) then
        col = utf8.len(lines[row]) + 1
      end
    end
  elseif key == 80 then -- Left
    if col > 1 then
      col = col - 1
    end
  elseif key == 79 then -- Right
    if col <= utf8.len(lines[row]) then
      col = col + 1
    end
  elseif key == 42 then -- BS
    if col > 1 then
      col = col - 1
      lines[row] = removeChar(lines[row], col)
    end
  else
    if code and 32 <= code and code <= 126 then
      lines[row] = insertChar(lines[row], c, col)
      col = col + 1
    end
  end
  draw()
end
function onChar(c)
  lines[row] = insertChar(lines[row], c, col)
  col = col + 1
  draw()
end
