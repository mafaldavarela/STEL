filename = 'values.txt';
values = importdata(filename);
filename = 'settings.txt';
settings = importdata(filename);
t = num2cell(settings);
[lambda, interval, intervals_nr, samples_nr, samples_avg] = deal(t{:});
categories = string(zeros(1, intervals_nr));
%create categories
for i=1 : 1 : intervals_nr
    categories(i)= string((i-1)*interval) + " - " + string((i)*interval);
end

set(gcf,'units','normalized','outerposition',[0 0 1 1]); %maximize plot
values = values';
figure1 = bar(categorical(categories), values);
title('Histogram');
xlabel('Intervals'); % x-axis label
ylabel('Quantity'); % y-axis label
dim = [.725 .68 .2 .2];
str = {'Lambda = '+string(lambda)
    'Interval used = '+ string(interval)
    'Nr. of intervals = '+ string(intervals_nr)
    'Samples used = '+ string(samples_nr)
    'Samples avg. = '+ string(samples_avg)
    'Expected avg. = '+ string(1/lambda)};
annotation('textbox',dim,'String',str,'FitBoxToText','on');
saveas(figure1,'picture.png')
