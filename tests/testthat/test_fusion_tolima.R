context("FusionTolima")

data(tolima)

get_numeric_data <- function() {
  return(tolima[, sapply(tolima, is.numeric)])
}

get_macronutrients_fis <- function() {
  return(macronutrients_fis)
}

get_macronutrients_aggreg <- function() {
  balance_gap <- NewFusionInput("Bal_Gap", NewMfTrapezoidalInf(0, 0.5), "BalanceGap")
  potassium <- NewFusionInput("K", NewMfTrapezoidalSup(0.2, 0.6), "K")
  phosphorus <- NewFusionInput("P", NewMfTrapezoidalSup(5, 15), "P")
  n_gap <- NewFusionInput("N_Gap", NewMfTriangular(0.5, 1, 1.5), "Ngap_N_OpN")
  base_sat <- NewFusionInput("Base_Sat", NewMfTrapezoidalSup(0.4, 0.6), "Base_S")

  # see macronutrients rulebase in [@denys20]
  macronutrients_fis <- NewFisFusion(
    "MacN", # FIS name
    c("Bal_Gap", "K", "P", "N_Gap", "Base_Sat"), # FIS inputs names
    c(2, 2, 2, 2, 2), # FIS inputs granularities
    "MacN", # FIS output name
    c(
      0, 0.1, 0.15, 0.2, 0.25, 0.35, 0.4, 0.45,
      0.3, 0.4, 0.45, 0.5, 0.55, 0.65, 0.7, 0.75,
      0.4, 0.3, 0.4, 0.45, 0.5, 0.55, 0.65, 0.7,
      0.55, 0.6, 0.7, 0.75, 0.8, 0.85, 0.9, 1
    ) # FIS conclusions
  )

  macronutrients_aggreg <- NewFusionAggreg(
    "MacN",
    NewAggregFis(macronutrients_fis),
    balance_gap, potassium, phosphorus, n_gap, base_sat
  )
  return(macronutrients_aggreg)
}

get_nutri_balance_aggreg <- function() {
  ph <- NewFusionInput("ph", NewMfTrapezoidal(5, 5.5, 6.5, 7.5), "pH")
  org_matter <- NewFusionInput("OM", NewMfTrapezoidalSup(3, 5), "OM")

  # see nutri balance rulebase in [@denys20]
  nutri_balance_fis <- NewFisFusion(
    "Nutri_Bal", # FIS name
    c("pH", "OM", "MacN"), # FIS inputs names
    c(2, 2, 2), # FIS inputs granularities
    "Nutri_Bal", # FIS output name
    c(0, 0.2, 0.3, 0.5, 0.4, 0.6, 0.7, 1.0) # FIS conclusions
  )

  nutri_balance_aggreg <- NewFusionAggreg(
    "Nutri_Bal",
    NewAggregFis(nutri_balance_fis),
    ph, org_matter, get_macronutrients_aggreg()
  )
  return(nutri_balance_aggreg)
}

get_chemical_aggreg <- function() {
  cadmium <- NewFusionInput("Cd", NewMfTrapezoidalInf(0, 0.43), "Cadmium")
  # see chemical WAM weights in [@denys20]
  chemical_aggreg <- NewFusionAggreg(
    "Chemical",
    NewAggregWam(weights = c(0.3, 0.7)),
    cadmium, get_nutri_balance_aggreg()
  )
  return(chemical_aggreg)
}

get_fusion <- function() {
  fusion <- NewFusion(tolima)
  fusion$aggregate <- get_chemical_aggreg()
  return(fusion)
}

check_macronutrients <- function(ag) {
  # see expected macronutrients in [@denys20]
  expect_equal(ag$MacN[1:10], c(0.696, 0.414, 0.337, 0.669, 0.470, 0.484, 0.559, 0.773, 0.380, 0.614), tolerance = 1e-3)
}

check_chemical <- function(ag) {
  # see expected chemical in [@denys20]
  expect_equal(ag$Chemical[1:10], c(0.459, 0.41, 0.467, 0.374, 0.773, 0.7, 0.651, 0.547, 0.421, 0.666), tolerance = 1e-3)
}

test_that("Tolima fusion", {
  skip_fusion_test()
  fusion <- get_fusion()
  fusion$perform()
  output <- fusion$output()
  check_macronutrients(output)
  check_chemical(output)
})

test_that("Tolima aggregate node", {
  skip_fusion_test()
  macronutrients <- .aggregate_node(get_numeric_data(), get_macronutrients_aggreg())
  check_macronutrients(macronutrients)
  chemical <- .aggregate_node(get_numeric_data(), get_chemical_aggreg())
  check_macronutrients(chemical)
  check_chemical(chemical)
})
