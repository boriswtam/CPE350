--This is configured for the I2C Bus on EIO4(SCL) and EIO5(SDA)
--Outputs data to Registers:
--X accel = 46000
--Y accel = 46002
--Z accel = 46004
--Local Temp = 46006
--Object Temp = 46008

fwver = MB.R(60004, 3)
devType = MB.R(60000, 3)
if (fwver < 1.0224 and devType == 7) or (fwver < 0.2037 and devType == 4) then
  print("This lua script requires a higher firmware version (T7 Requires 1.0224 or higher, T4 requires 0.2037 or higher). Program Stopping")
  MB.W(6000, 1, 0)
end

function convert_16_bit(msb, lsb, conv)--Returns a number, adjusted using the conversion factor. Use 1 if not desired  
  res = 0
  if msb >= 128 then
    res = (-0x7FFF+((msb-128)*256+lsb))/conv
  else
    res = (msb*256+lsb)/conv
  end
  return res
end

ADXL345_ADDR = 0x53 -- Accelometer address
TMP007_ADDR = 0x40 -- Temperature address
I2C.config(13, 12, 65516, 0, ADXL345_ADDR, 0)--configure the I2C Bus

addrs = I2C.search(0, 127)
addrsLen = table.getn(addrs)
found = 0
for i=1, addrsLen do--verify that the target device was found     
  if addrs[i] == ADXL345_ADDR then
    print("I2C ADXL345 Acceleromter Slave Detected")
    found = found+1
  end
  if addrs[i] == TMP007_ADDR then
    print("I2C TMP007_ADDR Slave Detected")
    found = found+1
  end
end


if found ~= 2 then
  print(string.format("%d", found).." slave devices found (looking for 4 devices), program stopping")
  MB.W(6000, 1, 0)
end

--init ADXL345 slave
MB.W(5104, 0, ADXL345_ADDR)--change target to acceleromter
I2C.write({0x31, 0x09})--set for +-4G
I2C.write({0x2D, 0x08})--Disable power saving

--init TMP007 slave
MB.W(5104, 0, TMP007_ADDR)--change target to temperature sensor
I2C.write({0x02, 0x3000 or 0x0100 or 0x0040})-- or 9600})
I2C.write({0x05, 0x8000 or 0x4000})

--Begin loop
LJ.IntervalConfig(0, 500)
stage = 0
while true do
  if LJ.CheckInterval(0) then
    MB.W(5104, 0, ADXL345_ADDR)--change target to accelerometer
    I2C.write({0x32})
 		ADXL345_raw_data = I2C.read(6)
		data_ADXL345 = {}
		for i=0, 2 do
     	 	table.insert(data_ADXL345, convert_16_bit(ADXL345_raw_data[(2*i)+2], ADXL345_raw_data[(2*i)+1], 233))
		end
		
		MB.W(46000, 3, data_ADXL345[1])--add X value, in Gs, to the user_ram register
    MB.W(46002, 3, data_ADXL345[2])--add Y
    MB.W(46004, 3, data_ADXL345[3])--add Z
        
	  print("X", data_ADXL345[1])
	  print("Y", data_ADXL345[2])
	  print("Z", data_ADXL345[3])
	  print("-----------")
	  
			 --begin TMP007 data read--
    MB.W(5104, 0, TMP007_ADDR)
    I2C.write({0x01})
    TMP007_raw_data = I2C.read(2)
    data_TMP007 = {}
    table.insert(data_TMP007, TMP007_raw_data[1])
	  MB.W(46006, 3, data_TMP007[1])
	  print("Local Temp: ", data_TMP007[1])
      
    I2C.write({0x03})
    TMP007_raw_data = I2C.read(2)
    data_TMP007 = {}
    table.insert(data_TMP007, TMP007_raw_data[1])
	  MB.W(46008, 3, data_TMP007[1])

	  print("Object Temp: ", data_TMP007[1])
	  print("-----------")
  end
end
