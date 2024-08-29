library(sf)
library(foreach)

get_spatial_collection <- function(wkt, type, crs) {
  geometries <- st_as_sfc(wkt, crs = crs) %>% st_collection_extract(type)
  # sp Spatial objects has numbered ID, "IDx" in sf
  # assign IDs to be compatible with sp
  return(as_Spatial(geometries, IDs = as.character(seq_along(geometries))))
}


get_spatial_polygons <- function(wkt, crs) {
  spatial_polygons <- get_spatial_collection(wkt, "POLYGON", crs)
  # sp SpatialPolygons has c("x", "y") colnames for matrix coords, no colnames in sf
  # assign colnames to be compatible with sp
  foreach(i = seq_along(spatial_polygons@polygons)) %do% {
    foreach(j = seq_along(spatial_polygons@polygons[[i]]@Polygons)) %do% {
      colnames(spatial_polygons@polygons[[i]]@Polygons[[j]]@coords) <- c("x", "y")
    }
  }
  return(spatial_polygons)
}

get_spatial_lines <- function(wkt, crs) {
  spatial_lines <- get_spatial_collection(wkt, "LINESTRING", crs)
  # sp SpatialLines has c("x", "y") colnames for matrix coords, no colnames in sf
  # assign colnames to be compatible with sp
  foreach(i = seq_along(spatial_lines@lines)) %do% {
    foreach(j = seq_along(spatial_lines@lines[[i]]@Lines)) %do% {
      colnames(spatial_lines@lines[[i]]@Lines[[j]]@coords) <- c("x", "y")
    }
  }
  return(spatial_lines)
}

get_spatial <- function(wkt, crs) {
  geometry <- st_as_sfc(wkt, crs = crs)
  return(as_Spatial(geometry))
}

get_spatial_points <- function(wkt, crs) {
  points <- get_spatial(wkt, crs)
  # sp SpatialPoints has c("x", "y") colnames for matrix coords, no colnames and rownames in sf
  # assign colnames and rownames to be compatible with sp
  colnames(points@coords) <- c("x", "y")
  rownames(points@coords) <- seq_along(points)
  return(points)
}
