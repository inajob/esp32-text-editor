putstring("hello from lua!")
putstring("hello from lua!")

lines = {""}
topRow = 1;
row = 1
col = 1

-- below line is `mode`
-- below line 2 is `henkan`
lastRow = getmaxline() - 3

globalKey = 0
globalCode = ""
fontH = 12
fontW = 6

function draw()
  setcursor(0, 0)
  -- redraw all lines
  cursorY = 0
  cursorX = 0
  posY = 0
  posX = 0
  for i, line in pairs(lines) do
    if i < topRow then
      goto continue
    end
    setcursor(posX, posY)
    -- clear line
    setcolor(0,0,0, 0,0,0);
    fillrect(0, posY, 320, fontH)
    setcolor(255,255,255, 0,0,0);
    -- draw string
    count = 0;
    hit = false
    for p, cc in utf8.codes(line) do
      c = utf8.char(cc)
      if i == row and count +1 == col then
        setcolor(0,0,0, 0,255,0);
        hit = true
        putstring(c)
        setcolor(255,255,255, 0,0,0);
        cursorX = posX
        cursorY = posY
      else
        putstring(c)
      end
      posX = posX + gettextwidth(c)
      if posX > 320 - fontW*2 then -- fold
        posX = 0
        posY = posY + fontH
        setcursor(posX, posY)
        -- clear line
        setcolor(0,0,0, 0,0,0);
        fillrect(0, posY, 320, fontH)
        setcolor(255,255,255, 0,0,0);
      end
      count = count + 1
    end
    if i == row and not(hit) then
      setcolor(0,0,0, 0,255,0);
      putstring("*")
      setcolor(255,255,255, 0,0,0);
      cursorX = posX
      cursorY = posY
    end
    posY = posY + fontH
    posX = 0;
    ::continue::
  end

  -- draw status line for heap
  setcolor(0,0,255, 0,0,0);
  setcursor(0, (lastRow-1)*fontH)
  putstring("heap: " .. getfreeheap())
  -- draw status line for char
  fillrect(0, (lastRow)*fontH, 320, fontH)
  setcolor(0,0,0, 0,0,255);
  setcursor(0, (lastRow)*fontH)
  putstring("key: ")
  putstring(globalKey)
  putstring(",code of c: ")
  if globalCode then
    putstring("" .. globalCode)
  else 
    putstring("nil")
  end 
  putstring(",row " .. row)
  putstring(",cow " .. col)
  putstring(",topRow " .. topRow)

  -- setcursor for fep
  setcursor(cursorX, cursorY)
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
    if row - topRow > lastRow - 2 then
      topRow = topRow + 1
    end

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
    if row < topRow then
      topRow = topRow - 1
    end
  elseif key == 81 then -- Down
    if row < #lines then
      row = row + 1
      if col >= utf8.len(lines[row]) then
        col = utf8.len(lines[row]) + 1
      end
    end
    if row - topRow > lastRow - 2 then
      topRow = topRow + 1
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
