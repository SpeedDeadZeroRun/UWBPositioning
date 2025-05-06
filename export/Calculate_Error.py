import numpy as np

def parse_file(filename):
    """
    Parses the input file to extract measure_location and guess_location data.
    """
    with open(filename, 'r') as file:
        lines = file.readlines()

    measure_location = None
    guess_locations = []
    found_measure = False

    for i, line in enumerate(lines):
        clean_line = line.strip().lower()  # Normalize line for matching
        if clean_line == "measure locations:":
            # Extract the next line for measure_location
            found_measure = True
            measure_location = [float(x) for x in lines[i + 1].strip().split()]
        elif clean_line == "guess_location:":
            # Extract all subsequent guess_location lines
            for subsequent_line in lines[i + 1:]:
                subsequent_line = subsequent_line.strip()
                if not subsequent_line or subsequent_line.startswith("--"):
                    break
                try:
                    guess_locations.append([float(x) for x in subsequent_line.split()])
                except ValueError:
                    # Skip invalid lines that cannot be parsed into floats
                    continue

    if not found_measure:
        raise ValueError("The file does not contain the 'measure locations:' line.")

    if measure_location is None or not guess_locations:
        raise ValueError("Failed to parse 'measure locations' or 'guess_location' data.")

    return np.array(measure_location), np.array(guess_locations)

def calculate_metrics(measure_location, guess_locations):
    """
    Calculates MSE, MAE, RMSE, SD, and Variance for each dimension (X, Y, Z).
    """
    errors = guess_locations - measure_location

    # print("Errors (Guess - Measure):")
    # print(errors)

    mse = np.mean(errors**2, axis=0)  # Mean squared error
    mae = np.mean(np.abs(errors), axis=0)  # Mean absolute error
    rmse = np.sqrt(mse)  # Root mean squared error
    sd = np.std(guess_locations, axis=0)  # Standard deviation
    variance = np.var(guess_locations, axis=0)  # Variance

    # print("Absolute Errors:")
    # print(np.abs(errors))
    # print("MAE Calculations:")
    # print("Sum of Absolute Errors:", np.sum(np.abs(errors), axis=0))
    # print("MAE:", mae)

    return mse, mae, rmse, sd, variance


def main():
    input_file = "elimination_measure_pos8_export.txt"  # Use raw string for Windows file path
    measure_location, guess_locations = parse_file(input_file)
    mse, mae, rmse, sd, variance = calculate_metrics(measure_location, guess_locations)

    print("Metrics for X, Y, Z:")
    print(f"MSE: {mse}")
    print(f"MAE: {mae}")
    print(f"RMSE: {rmse}")
    print(f"Standard Deviation: {sd}")
    print(f"Variance: {variance}")
    print("Measure Location:", measure_location)
    # print("Guess Locations:", guess_locations)
    print("Number of Guess Locations:", len(guess_locations))


if __name__ == "__main__":
    main()
