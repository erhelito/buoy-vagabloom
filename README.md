# Buoy vagabloom

This is a school's project. We made a buoy, which collect data in the ocean, like temperature, salinity, etc... These data are collected, and then sent to the satellite.
It will help us to understand the ocean, phytoplankton, and other interesting stuff.

Our scientific team is working on biodiversity, and in the technical part, we are creating this buoy.

Currently, we have to finish the software.

# Todo
- [x] backup data in dataframe
- [ ] accelerometer
- [x] temperature (surface and depth)
- [x] salinity
- [x] impermeability
- [x] battery
- [ ] hera card
- [ ] compute average data

# Others
## Data frame
The data frame is a table, which contains all the data.
It is saved in this way :
`[accelerometer, surface_temperature, depth_temperature, salinity, impermeability, battery]`

So :
- `accelerometer` is `data[0]`
- `surface_temperature` is `data[1]`
- `depth_temperature` is `data[2]`
- `salinity` is `data[3]`
- `impermeability` is `data[4]`
- `battery` is `data[5]`

Each time we collect data, we add a new line in a new data frame.
This way :

```arduino
float actual_data[5];
float past_data[5];
```