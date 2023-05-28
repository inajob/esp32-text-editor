putstring("hello from lua!")
putstring("hello from lua!")

fileName = "/test.txt"
history = {}
lines = {""}
topRow = 1;
row = 1
col = 1
statusLine = ""
yank = ""

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
    if i - topRow > lastRow then
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
  setcolor(0,0,0, 0,0,0);
  fillrect(0, posY, 320, fontH)
  setcolor(255,255,255, 0,0,0);

  -- draw status line for heap
  setcolor(0,0,255, 0,0,0);
  fillrect(0, (lastRow - 1)*fontH, 320, fontH)
  setcolor(255,255,255, 0,0,255);
  setcursor(0, (lastRow-1)*fontH)
  putstring("heap: " .. getfreeheap())
  putstring(" " .. statusLine)
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
  putstring(",fileName " .. fileName)

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

function saveFile(fn)
  savefile(fn, table.concat(lines, "\n"))
end
function loadFile(fn)
  if exists(fn) then
    body = readfile(fn)
    lines = {}
    line = ""
    for p, lc in utf8.codes(body) do
      c = utf8.char(lc)
      if c == "\n" then
        lines[#lines + 1] = line
        line = ""
        goto continue
      end
      line = line .. c
      ::continue::
    end
    lines[#lines + 1] = line
    statusLine = "load " .. fileName
  else
    lines = {""}
    col = 1
    row = 1
    statusLine = "new " .. fileName
  end
  topRow = 1
  col = 1
  row = 1
  setcolor(0,0,0, 0,0,0)
  fillrect(0, 0, 320, 240)

end

function keydown(key, c, ctrl)
  code = string.byte(c)
  globalKey = key
  globalCode = code
  if ctrl then
    if c == "s" then
      saveFile(fileName)
      statusLine = "saved " .. fileName
    elseif c == "c" then
      statusLine = "check" .. exists(fileName)
    elseif c == "x" then
      exit()
      statusLine = "exit"
    elseif c == "l" then
      loadFile(fileName)
    elseif c == "k" then
      yank = lines[row]
      table.remove(lines, row)
    elseif c == "u" then
      table.insert(lines, row, yank)
    elseif c == "g" then
      -- seek wiki name
      nextFile = ""
      hit = false
      found = false
      count = 0
      for p, cc in utf8.codes(lines[row]) do
        c = utf8.char(cc)
        if hit then
          if count == col then
            found = true
          end
          if c == "[" then
            found = false
            break
          elseif c == "]" then
            hit = false
            if found then
              break
            else
              nextFile = ""
            end
          else
            nextFile = nextFile .. c
          end
        else
          if c == "[" then
            hit = true
          end
        end
        count = count + 1
      end
      if found then
        statusLine = "next file is: " .. nextFile
        debug("next file is: " .. nextFile)
        table.insert(history, fileName)
        fileName = "/" .. nextFile .. ".txt"
        loadFile(fileName)
      end
    elseif c == "b" then
      if #history > 0 then
        fileName = history[#history]
        table.remove(history)
        loadFile(fileName)
      end
    end
    draw()
    return
  end
  if code == 13 then
    newLine = ""
    newLineNext = ""
    count = 1
    for p, lc in utf8.codes(lines[row]) do
      if count < col then
        newLine = newLine .. utf8.char(lc)
      else
        newLineNext = newLineNext .. utf8.char(lc)
      end
      count = count + 1
    end
    lines[row] = newLine
    table.insert(lines, row + 1, "")
    row = row + 1
    lines[row] = newLineNext
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
    if row - topRow > lastRow - 4 then
      topRow = topRow + 1
    end

  elseif key == 80 then -- Left
    if col == 1 then
      if row > 1 then
        row = row - 1
        col = utf8.len(lines[row]) + 1
      end
    else
      col = col - 1
    end
  elseif key == 79 then -- Right
    if col == utf8.len(lines[row]) + 1 and row < #lines then
      col = 1
      row = row + 1
    else
      col = col + 1
    end
  elseif key == 42 then -- BS
    if col == 1 then
      if row > 1 then
        -- merge line
        nowLine = lines[row]
        table.remove(lines, row)
        row = row - 1
        col = utf8.len(lines[row]) + 1
        lines[row] = lines[row] .. nowLine
      end
    else
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

draw()
