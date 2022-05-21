# Buoy vagabloom

This is a school's project. We made a buoy, which collect data in the ocean, like temperature, salinity, etc... These data are collected, and then sent to the satellite.
It will help us to understand the ocean, phytoplankton, and other interesting stuff.

Our scientific team is working on biodiversity, and in the technical part, we are creating this buoy.

Currently, we have to finish the software.

# Todo
- [ ] accelerometer
- [ ] temperature
- [x] salinity
- [x] impermeability
- [x] battery
- [ ] hera card

# Others
## Data frame
The data frame is a table, which contains all the data.
It is saved in this way :
`[accelerometer, temperature, salinity, impermeability, battery]`

So :
- `accelerometer` is `data[0]`
- `temperature` is `data[1]`
- `salinity` is `data[2]`
- `impermeability` is `data[3]`
- `battery` is `data[4]`

Each time we collect data, we add a new line in a new data frame.
This way :

```arduino
float actual_data[5];
float past_data[5];
```