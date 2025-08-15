import { useEffect, useState } from "react";
import {
  LineChart,
  Line,
  CartesianGrid,
  XAxis,
  YAxis,
  Tooltip,
} from "recharts";

export default function HomePage() {
  const [temperatur, setTemperatur] = useState(null);
  const [tryk, setTryk] = useState(null);
  const [luftfugtighed, setLuftfugtighed] = useState(null);
  const [createdAt, setCreatedAt] = useState(null);
  const [tempChartData, setTempChartData] = useState([]);
  const [trykChartData, setTrykChartData] = useState([]);
  const [luftChartData, setLuftChartData] = useState([]);
  const [fetchError, setFetchError] = useState(false);
  useEffect(() => {
    const fetchData = () => {
      fetch("/api/vejrdata")
        .then((response) => {
          if (!response.ok) {
            setFetchError(true);
          }
          return response.json();
        })
        .then((data) => {
          setFetchError(false);
          const temp = data[0]?.temperatur ?? 0;
          const tryk = data[0]?.tryk ?? 0;
          const luft = data[0]?.luftfugtighed ?? 0;
          const tid = data[0]?.tidspunkt ?? new Date().toISOString();

          setTemperatur(temp);
          setTryk(tryk);
          setLuftfugtighed(luft);
          setCreatedAt(tid);

          setTempChartData((prev) => [
            ...prev,
            {
              name: new Date(tid).toLocaleTimeString(),
              value: temp,
            },
          ]);
          setTrykChartData((prev) => [
            ...prev,
            {
              name: new Date(tid).toLocaleTimeString(),
              value: tryk,
            },
          ]);
          setLuftChartData((prev) => [
            ...prev,
            {
              name: new Date(tid).toLocaleTimeString(),
              value: luft,
            },
          ]);
        })
        .catch((error) => {
          setFetchError(true);
          console.error("Error fetching weather data:", error);
        });
    };
    fetchData();
    const interval = setInterval(fetchData, 10000);
    return () => clearInterval(interval);
  }, []);

  return (
    <div>
      <h1>Welcome to the Weather Data App </h1>
      {fetchError && (
        <div className="text-red-500 font-bold text-xl mb-4">
          Fetching data failed
        </div>
      )}
      <div className="flex flex-wrap">
        <div className="rows justify-between w-107 mt-10 border rounded-lg h-auto p-4 m-10">
          <p className="text-l font-bold text-white p-6">
            🥵Temperatur:
            {temperatur !== null ? `${temperatur}°C` : "Loading..."}
          </p>
          <p className="text-l font-bold text-white p-6">
            🌬️Tryk: {tryk !== null ? `${tryk} hPa` : "Loading..."}
          </p>
          <p className="text-l font-bold text-white p-6">
            🌫️Luftfugtighed:
            {luftfugtighed !== null ? `${luftfugtighed}%` : "Loading..."}
          </p>
          <p className="text-l font-bold text-white p-6">
            🕔Tidspunkt:
            {createdAt !== null
              ? new Date(createdAt).toLocaleTimeString()
              : "Loading..."}
          </p>
        </div>
        <div className="w-1/2 mt-10 ml-26">
          <h1 className="text-2xl font-bold text-white pl-20">Temperatur:</h1>
          <LineChart
            width={500}
            height={300}
            data={tempChartData}
            margin={{ top: 5, right: 30, left: 20, bottom: 5 }}
          >
            <CartesianGrid strokeDasharray="3 3" />
            <XAxis dataKey="name" />
            <YAxis
              domain={[(dataMin) => dataMin - 0.5, (dataMax) => dataMax + 0.5]}
            />
            <Tooltip />
            <Line type="monotone" dataKey="value" stroke="#8884d8" />
          </LineChart>
        </div>
        <div className="w-1/2 mt-10">
          <h1 className="text-2xl font-bold text-white pl-20">Tryk:</h1>
          <LineChart
            width={500}
            height={300}
            data={trykChartData}
            margin={{ top: 5, right: 30, left: 20, bottom: 5 }}
          >
            <CartesianGrid strokeDasharray="3 3" />
            <XAxis dataKey="name" />
            <YAxis
              domain={[(dataMin) => dataMin - 0.5, (dataMax) => dataMax + 0.5]}
            />
            <Tooltip />
            <Line type="monotone" dataKey="value" stroke="#82ca9d" />
          </LineChart>
        </div>
        <div className="w-1/2 mt-10">
          <h1 className="text-2xl font-bold text-white pl-20">
            Luftfugtighed:
          </h1>
          <LineChart
            width={500}
            height={300}
            data={luftChartData}
            margin={{ top: 5, right: 30, left: 20, bottom: 5 }}
          >
            <CartesianGrid strokeDasharray="3 3" />
            <XAxis dataKey="name" />
            <YAxis
              domain={[(dataMin) => dataMin - 0.5, (dataMax) => dataMax + 0.5]}
            />
            <Tooltip />
            <Line type="monotone" dataKey="value" stroke="#ffc658" />
          </LineChart>
        </div>
      </div>
    </div>
  );
}
