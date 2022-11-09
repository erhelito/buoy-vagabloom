# Buoy vagabloom

This is a school's project. We made a buoy, which collect data in the ocean, like temperature, salinity, etc... These data are collected, and then sent to the satellite.
It will help us to understand the ocean, phytoplankton, and other interesting stuff.

Our scientific team is working on biodiversity, and in the technical part, we are creating this buoy.

Currently, we have to finish the programming of our buoy.

# Todo
- [x] backup data in dataframe
- [x] accelerometer
- [x] temperature (surface and depth)
- [x] salinity
- [x] impermeability
- [x] battery
- [ ] hera card
- [x] compute average data
- [x] create file sensor_only (for test)
- [x] test sensor_only
- [ ] repair salinity sensor

# Others
## Data frame
The data frame is a table, which contains all the data.
Structure
`[accelerometer, surface_temperature, depth_temperature, salinity, impermeability, battery]`

So :
- `accelerometer` is `data[0]`
- `surface_temperature` is `data[1]`
- `depth_temperature` is `data[2]`
- `salinity` is `data[3]`
- `impermeability` is `data[4]`
- `battery` is `data[5]`

Each time we collect data, previous data are stored in `past_data`.